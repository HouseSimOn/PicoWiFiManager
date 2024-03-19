#pragma once

#include "pico/stdlib.h"
#include <stdint.h>
#include <stdbool.h>
#include "pico/cyw43_arch.h"

/// @brief Typedef of function that can be set as callback when network is found
typedef int (*WiFi_ScanCallback)(void *, const cyw43_ev_scan_result_t *);

/// @brief Enables AP mode.
/// @param Ssid of AP mode.
/// @param Password of AP mode.
void WiFi_SetApMode(char *Ssid, char *Password);

/// @brief Enables STA mode.
void WiFi_SetStaMode();

/// @brief Disables STA mode.
void WiFi_DisableStaMode();

/// @brief Disables AP mode.
void WiFi_DisableApMode();

/// @brief Connects to the AP with timeout - blocking function.
/// @param Ssid of network to connect to
/// @param Password of network
/// @param Timeout to give up connecting.
/// @return true if connected successfully
bool WiFi_ConnectTimeout(char *Ssid, char *Password, uint32_t Timeout);

/// @brief Starts connecting to AP - async function
/// @param Ssid of network to connect to
/// @param Password of network
/// @return true if started successfully.
bool WiFi_Connect(char *Ssid, char *Password);

/// @brief Disassociate from a wifi network in STA mode.
/// @return true if success.
bool WiFi_DisconnectSTA();

/// @brief Disassociate from a wifi network in AP mode.
/// @return true if success.
bool WiFi_DisconnectAP();

/// @brief Returns true if connected to AP in STA mode.
/// @return bool
bool WiFi_IsConnectedToAp();

/// @brief Returns true if disconnected to AP in STA mode.
/// @return bool
bool WiFi_IsStaDisconnected();

/// @brief Starts scan of WiFi networks
/// @param Callback called when network found
/// @return true if started successfully
bool WiFi_ScanNetworks(WiFi_ScanCallback Callback);

/// @brief Returns true if scan is in active state.
/// @return bool
bool WiFi_IsScanActive();
