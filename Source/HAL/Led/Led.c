#include "Led.h"

#if defined RASPBERRYPI_PICO_W
#include "pico/cyw43_arch.h"
#endif

#if defined RASPBERRYPI_PICO_W

void Led_Init() {}

#else

void Led_Init()
{
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
}

#endif

#if defined RASPBERRYPI_PICO_W

void Led_SetState(bool State)
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, State);
}

#else

void Led_SetState(bool State)
{
    gpio_put(25, State);
}

#endif
