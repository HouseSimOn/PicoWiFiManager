#include "WiFi.h"

void WiFi_SetApMode(char *Ssid, char *Password) {
    cyw43_arch_enable_ap_mode(Ssid, Password, CYW43_AUTH_WPA2_AES_PSK);
}

void WiFi_SetStaMode() {
    cyw43_arch_enable_sta_mode();
}

void WiFi_DisableStaMode() {
    cyw43_arch_disable_sta_mode();
}

void WiFi_DisableApMode() {
    cyw43_arch_disable_ap_mode();
}

bool WiFi_ConnectTimeout(char *Ssid, char *Password, uint32_t Timeout) {
    if(cyw43_arch_wifi_connect_timeout_ms(Ssid, Password, CYW43_AUTH_WPA2_AES_PSK, Timeout)) {
        return false;
    }

    return true;
}

bool WiFi_Connect(char *Ssid, char *Password) {
    int ret = cyw43_arch_wifi_connect_async(Ssid, Password, CYW43_AUTH_WPA2_AES_PSK);
    return !(bool)ret;
}

bool WiFi_DisconnectSTA() {
    int ret = cyw43_wifi_leave(&cyw43_state, CYW43_ITF_STA);
    return !(bool)ret;
}

bool WiFi_DisconnectAP() {
    int ret = cyw43_wifi_leave(&cyw43_state, CYW43_ITF_AP);
    return !(bool)ret;
}

bool WiFi_IsConnectedToAp() {
    if(cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA) == CYW43_LINK_UP) {
        return true;
    }

    return false;
}

bool WiFi_IsStaDisconnected() {
    if(cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA) == CYW43_LINK_DOWN) {
        return true;
    }

    return false;
}

bool WiFi_ScanNetworks(WiFi_ScanCallback Callback) {
    if(Callback == NULL) {
        return false;
    }

    cyw43_wifi_scan_options_t scanOptions = {0};

    return !(bool)(cyw43_wifi_scan(&cyw43_state, &scanOptions, NULL, Callback));
}

bool WiFi_IsScanActive() {
    return cyw43_wifi_scan_active(&cyw43_state);
}
