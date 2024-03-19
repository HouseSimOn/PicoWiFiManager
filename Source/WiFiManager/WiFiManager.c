#include "WiFiManager.h"

#include "Logger.h"
#include "WiFi.h"
#include "dhcpserver.h"
#include "lwip/ip_addr.h"
#include "pico/cyw43_arch.h"
#include "hardware/flash.h"
#include <hardware/sync.h>

#define MAX_NETWORKS_COUNT  10

typedef enum {
    Init = 0,
    ApMode,
    ApModeIdle,

    WaitTillStaDisconnect,

    ConnectAp,
    WaitForConnection,
    StaModeIdle,
} WiFiManager_State_t;

// size of struct must be N*256
typedef struct {
    uint8_t StoredFlag;
    char Ssid[128];
    char Password[127];
} NetworkCredentials_t;

static bool isNetworkAdded(uint8_t *Bssid);
static int scanResultCallback(void *Data, const cyw43_ev_scan_result_t *Result);
static bool credendialsStored();
static void storeInFlash(char *Ssid, char *Password);
static bool requestNetworkScan();
static void apDeactivate();
static void staDeactivate();

volatile const NetworkCredentials_t __attribute__((section (".flashDataNetwork"))) NetworkCredentials;

static struct {
    WiFiManager_State_t State;
    struct {
        ip4_addr_t mask;
        ip_addr_t gw;
        dhcp_server_t dhcp_server;
    } Ap;
    struct {
        bool Started;
        uint8_t Count;
        WiFiNetworkInfo_t Data[MAX_NETWORKS_COUNT];
    } Scan;
} WiFiManagerContext = {.State = Init, .Scan.Started = false};

void WiFiManager_Perform() {
    if(WiFiManagerContext.Scan.Started) {
        if (!WiFi_IsScanActive()) {
            WiFiNetworkInfo_t *netInfo = NULL;

            LOG("Scan finished - found %d networks", WiFiManagerContext.Scan.Count);

            for (uint8_t i = 0; i < WiFiManagerContext.Scan.Count; i++) {
                netInfo = &WiFiManagerContext.Scan.Data[i];

                if (netInfo != NULL) {
                    LOG("Net: %d %s %x:%x:%x:%x:%x:%x %d", i, netInfo->Ssid, netInfo->Bssid[0], netInfo->Bssid[1],
                        netInfo->Bssid[2], netInfo->Bssid[3], netInfo->Bssid[4], netInfo->Bssid[5], netInfo->Rssi);
                }
            }
            
            WiFiManagerContext.Scan.Started = false;
        }

    }

    switch (WiFiManagerContext.State) {
    case Init:
        apDeactivate();
        staDeactivate();

        if (credendialsStored()) {
            LOG("Stored");
            WiFiManagerContext.State = WaitTillStaDisconnect;
        } else {
            LOG("Not Stored");
            WiFiManagerContext.State = ApMode;
        }
        break;

    case ApMode: {
        LOG("SetApMode");
        WiFi_SetApMode("PicoWiFiManager", "12345678");

        IP4_ADDR(ip_2_ip4(&WiFiManagerContext.Ap.gw), 192, 168, 4, 1);
        IP4_ADDR(ip_2_ip4(&WiFiManagerContext.Ap.mask), 255, 255, 255, 0);

        // Start the dhcp server
        LOG("dhcp_server_init");
        dhcp_server_init(&WiFiManagerContext.Ap.dhcp_server, &WiFiManagerContext.Ap.gw, &WiFiManagerContext.Ap.mask);
        
        requestNetworkScan();

        WiFiManagerContext.State = ApModeIdle;
        break;
    }

    case ApModeIdle: {

        break;
    }

    case WaitTillStaDisconnect: {
        if(WiFi_IsStaDisconnected()) {
            WiFi_SetStaMode();
            WiFiManagerContext.State = ConnectAp;
        }
        break;
    }

    case ConnectAp: {
        if(WiFi_Connect((char *)NetworkCredentials.Ssid, (char *)NetworkCredentials.Password)) {
            WiFiManagerContext.State = WaitForConnection;
        }
        break;
    }

    case WaitForConnection: {
        if(WiFi_IsConnectedToAp()) {
            WiFiManagerContext.State = StaModeIdle;

            apDeactivate();

            extern cyw43_t cyw43_state;
            uint32_t ip_addr = cyw43_state.netif[CYW43_ITF_STA].ip_addr.addr;
            LOG("IP Address: %ld.%ld.%ld.%ld\n", ip_addr & 0xFF, (ip_addr >> 8) & 0xFF, (ip_addr >> 16) & 0xFF, ip_addr >>
        24);
        }
        break;
    }

    case StaModeIdle:
        break;

    default:
        break;
    }
}

void WiFiManager_Connect(char *Ssid, char *Password) {
    storeInFlash(Ssid, Password);

    if(!WiFi_DisconnectSTA()) {
        LOG("WiFi_DisconnectSTA failed!");
    }

    WiFiManagerContext.State = WaitTillStaDisconnect;
}

bool WiFiManager_ScanNetworks() {
    return requestNetworkScan();
}

void WiFiManager_EraseStoredData() {
    uint32_t interrupts = save_and_disable_interrupts();
    flash_range_erase(((uint32_t)&NetworkCredentials-XIP_BASE), 4096);
    restore_interrupts(interrupts);
}

uint8_t WiFiManager_GetScannedNetworksCount() {
    return WiFiManagerContext.Scan.Count;
}

bool WiFiManager_GetScannedNetworkInfo(uint8_t Idx, WiFiNetworkInfo_t **Info) {
    if(Idx >= WiFiManagerContext.Scan.Count || Info==NULL)
        return false;

    *Info = &WiFiManagerContext.Scan.Data[Idx];

    return true;
}

static bool isNetworkAdded(uint8_t *Bssid) {
    if(WiFiManagerContext.Scan.Count==0)
        return false;
    for(uint8_t i=0;i<WiFiManagerContext.Scan.Count;i++) {
        if(memcmp(Bssid, WiFiManagerContext.Scan.Data[i].Bssid, sizeof(WiFiManagerContext.Scan.Data[i].Bssid))==0){
            return true;
        }
    }

    return false;
}

static int scanResultCallback(void *Data, const cyw43_ev_scan_result_t *Result) {
    (void)Data;

    // printf("Scan callback: %s\n", Result->ssid);

    if(WiFiManagerContext.Scan.Count>=MAX_NETWORKS_COUNT) {
        // printf("ScanNetworksData full!\n");
        return 0;
    }

    if(isNetworkAdded((uint8_t *)Result->bssid)) {
        // printf("Network duplicate. Not added.\n");
        return 0;
    }

    WiFiManagerContext.Scan.Data[WiFiManagerContext.Scan.Count].SsidLen = Result->ssid_len;
    memcpy(&WiFiManagerContext.Scan.Data[WiFiManagerContext.Scan.Count].Ssid, Result->ssid, Result->ssid_len);
    WiFiManagerContext.Scan.Data[WiFiManagerContext.Scan.Count].AuthMode = Result->auth_mode;
    WiFiManagerContext.Scan.Data[WiFiManagerContext.Scan.Count].Rssi = Result->rssi;
    memcpy(&WiFiManagerContext.Scan.Data[WiFiManagerContext.Scan.Count].Bssid, Result->bssid, sizeof(Result->bssid));

    WiFiManagerContext.Scan.Count++;

    return 0;
}

static bool credendialsStored() {
    return (NetworkCredentials.StoredFlag == 0xAA);
}

static void storeInFlash(char *Ssid, char *Password) {
    NetworkCredentials_t data;
    data.StoredFlag = 0xAA;
    strcpy(data.Ssid, Ssid);
    strcpy(data.Password, Password);

    uint32_t interrupts = save_and_disable_interrupts();

    // &NetworkCredentials-0x10000000
    // First parameter is index in flash memory. That's why XIP_BASE is used
    flash_range_erase(((uint32_t)&NetworkCredentials-XIP_BASE), 4096);
    flash_range_program(((uint32_t)&NetworkCredentials-XIP_BASE), (const uint8_t*)&data, sizeof(NetworkCredentials));

    restore_interrupts(interrupts);
}

static bool requestNetworkScan() {
    if(WiFi_ScanNetworks(scanResultCallback)) {
        memset(&WiFiManagerContext.Scan.Data, 0, sizeof(WiFiManagerContext.Scan.Data));
        WiFiManagerContext.Scan.Count = 0;
        WiFiManagerContext.Scan.Started = true;

        return true;
    }

    LOG("Request network scan failed");
    return false;
}

static void apDeactivate() {
    dhcp_server_deinit(&WiFiManagerContext.Ap.dhcp_server);
    WiFi_DisconnectAP();
    WiFi_DisableApMode();
}

static void staDeactivate() {
    WiFi_DisconnectSTA();
    WiFi_DisableStaMode();
}

