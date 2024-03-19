#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t Bssid[6]; ///< access point mac address
    uint8_t SsidLen;  ///< length of wlan access point name
    uint8_t Ssid[32]; ///< wlan access point name
    uint8_t AuthMode; ///< wifi auth mode \ref CYW43_AUTH_
    int16_t Rssi;     ///< signal strength
} WiFiNetworkInfo_t;

/// @brief Performs WiFiManager state machine
void WiFiManager_Perform();

/// @brief Connects to the WiFi AP. Stores passed credentials in flash memory
/// @param Ssid Ssid of WiFi network
/// @param Password WiFi password
void WiFiManager_Connect(char *Ssid, char *Password);

/// @brief Requests wifi networks scan
/// @return true if requested successfully
bool WiFiManager_ScanNetworks();

/// @brief Erases stored credentials in flash memory
void WiFiManager_EraseStoredData();

/// @brief Returns count of found networks during scanning
/// @return Count of detected network
uint8_t WiFiManager_GetScannedNetworksCount();

/// @brief Returns WiFiNetworkInfo_t of detected network during scan.
/// @param Idx Index of found network
/// @param Info Returned data
/// @return true if returned successfully, false otherwise
bool WiFiManager_GetScannedNetworkInfo(uint8_t Idx, WiFiNetworkInfo_t **Info);
