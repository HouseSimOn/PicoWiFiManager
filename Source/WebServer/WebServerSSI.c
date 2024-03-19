#include "WebServerSSI.h"
#include "Logger.h"
#include "WiFi.h"
#include "WiFiManager.h"
#include "lwip/apps/httpd.h"
#include <stdio.h>

#define SSI_TAGS_SIZE 12

typedef enum {
    SsiWiFiMode = 0,
    SsiWiFiConnectedAP,

    _SsiWiFiScanFirstItem,
    SsiWiFiScan1 = _SsiWiFiScanFirstItem,
    SsiWiFiScan2,
    SsiWiFiScan3,
    SsiWiFiScan4,
    SsiWiFiScan5,
    SsiWiFiScan6,
    SsiWiFiScan7,
    SsiWiFiScan8,
    SsiWiFiScan9,
    SsiWiFiScan10,
    _SsiWiFiScanLastItem = SsiWiFiScan10,
} SsiTags_t;

/// @brief Tags from webpage
const char *WebServerSSI_Tags[SSI_TAGS_SIZE] = {
    [SsiWiFiMode] = "WiFiMode", [SsiWiFiConnectedAP] = "WiFiConn", [SsiWiFiScan1] = "WiFi01",
    [SsiWiFiScan2] = "WiFi02",  [SsiWiFiScan3] = "WiFi03",         [SsiWiFiScan4] = "WiFi04",
    [SsiWiFiScan5] = "WiFi05",  [SsiWiFiScan6] = "WiFi06",         [SsiWiFiScan7] = "WiFi07",
    [SsiWiFiScan8] = "WiFi08",  [SsiWiFiScan9] = "WiFi09",         [SsiWiFiScan10] = "WiFi10"};

/// @brief Handler - changes tags to text
/// @param  Index - index of tag
/// @param  Insert - Text to be inserted on page
/// @param  InsertLength - Max size of Inserted page
/// @return Length of inserted text
uint16_t __time_critical_func(webServerSSI_handler)(int Index, char *Insert, int InsertLength) {
    uint16_t printed = 0;

    // LOG("SSI Handler - index: %d", Index);

    switch (Index) {
    case SsiWiFiMode: {
        printed = (uint16_t)snprintf(Insert, (size_t)InsertLength, "[Not-supported-feature]");
        break;
    }

    case SsiWiFiConnectedAP:
        printed = (uint16_t)snprintf(Insert, (size_t)InsertLength, "[Not-supported-feature]");
        break;

    default:
        break;
    }

    if (Index >= _SsiWiFiScanFirstItem && Index <= _SsiWiFiScanLastItem) {
        uint8_t idx = (uint8_t)Index - _SsiWiFiScanFirstItem;
        WiFiNetworkInfo_t *netInfo = NULL;

        if (idx >= WiFiManager_GetScannedNetworksCount())
            return 0;

        WiFiManager_GetScannedNetworkInfo(idx, &netInfo);
        if (netInfo != NULL) {
            printed = (uint16_t)snprintf(Insert, (size_t)InsertLength,
                                         "<tr><td>%s</td><td>%X:%X:%X:%X:%X:%X</td><td>%d</td></tr>", netInfo->Ssid,
                                         netInfo->Bssid[0], netInfo->Bssid[1], netInfo->Bssid[2], netInfo->Bssid[3],
                                         netInfo->Bssid[4], netInfo->Bssid[5], netInfo->Rssi);
        } else {
            return 0;
        }
    }

    return printed;
}

void WebServerSSI_Init() {
    http_set_ssi_handler(webServerSSI_handler, WebServerSSI_Tags, SSI_TAGS_SIZE);
}
