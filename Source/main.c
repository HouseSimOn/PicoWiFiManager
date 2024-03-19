#include "Commands.h"
#include "Led.h"
#include "Logger.h"
#include "WebServerCGI.h"
#include "WebServerSSI.h"
#include "WiFiManager.h"
#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"

#if defined RASPBERRYPI_PICO_W
#include "pico/cyw43_arch.h"
#endif

static volatile bool timerFired = false;
static repeating_timer_t timer;
static bool ledState = false;

bool timerCallback(repeating_timer_t *rt) {
    (void)rt;

    timerFired = true;

    return true;
}

int main() {
    // To fix debug
    timer_hw->dbgpause = 0;

    stdio_init_all();

    Commands_Init();

    LOG("Starting...\n");
    Led_Init();

#if defined RASPBERRYPI_PICO_W
    if (cyw43_arch_init() == 0) {
        LOG("[OK] cyw43_init()");
    } else {
        LOG("[NOK] cyw43_init()");
    }
#endif

    add_repeating_timer_ms(100, timerCallback, &ledState, &timer);

    httpd_init();
    WebServerSSI_Init();
    WebServerCGI_Init();

    while (true) {
        if (timerFired) {
            Led_SetState(ledState);
            ledState = !ledState;

            timerFired = false;
        }

        Commands_Perform();
        WiFiManager_Perform();
    }

    return 0;
}
