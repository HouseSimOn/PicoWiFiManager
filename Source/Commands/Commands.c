#include "Commands.h"

#include "Logger.h"
#include "pico/stdlib.h"
#include <stdint.h>
#include <stdio.h>

#include "WiFiManager.h"
#include "WiFi.h"

static volatile bool CharReceived = false;

static void charAvailableCallback(void *Data);
static void printHelp();

void Commands_Init() {
    stdio_set_chars_available_callback(charAvailableCallback, NULL);
    LOG("Type '?' to print commands help\n");
}

void Commands_Perform() {
    if (!CharReceived) {
        return;
    }

    int32_t command = getchar_timeout_us(0);
    if (command < 0) {
        return;
    }

    switch (command) {
    case '?':
        printHelp();
        break;

    case 's':
        LOG("Scan Networks command!");
        WiFiManager_ScanNetworks();
        break;

    case 'e': {
        LOG("Erase stored data command!");
        WiFiManager_EraseStoredData();
        break;
    }

    default:
        LOG("[NOK] Not known command - [%d]", (uint8_t)command);
        break;
    }

    CharReceived = false;
}

static void charAvailableCallback(void *Data) {
    (void)Data;

    CharReceived = true;
}

static void printHelp() {
    printf("\n___Command Help___\n");
    printf("'?' - to printf this help\n");
    printf("'e' - to erase stored data by WiFiManager\n");
    printf("'s' - to perform Wifi scan\n");
    printf("\n");
}