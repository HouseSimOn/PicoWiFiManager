#include "WebServerCGI.h"

#include "lwip/apps/httpd.h"
#include <stdio.h>
#include "WiFiManager.h"
#include "Logger.h"

/// @brief Handler that is called when on wifi SSID and Password form submit.
/// @param iIndex - index of handler from list
/// @param iNumParams - Number of parameters
/// @param pcParam - List of parameter names
/// @param pcValue - Parameter values
/// @return Path - response that is to be sent to the connected browser
static const char *wifiHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/// @brief List of handlers
static const tCGI cgiHandlersTable[] = {{.pcCGIName = "/wifi.cgi", .pfnCGIHandler = wifiHandler}};

void WebServerCGI_Init() {
    http_set_cgi_handlers(cgiHandlersTable, 1);
}

static const char *wifiHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    (void)iIndex;

    char *ssid = NULL;
    char *pass = NULL;

    if(iNumParams != 2) {
        LOG("wifiHandler - not expected iNumParams = %d.", iNumParams);
        return "/wifi.shtml";
    }

    LOG("Received params: %s, %s.", pcParam[0], pcParam[1]);

    ssid = pcValue[0];
    pass = pcValue[1];

    WiFiManager_Connect(ssid, pass);

    return "/wifi.shtml";
}
