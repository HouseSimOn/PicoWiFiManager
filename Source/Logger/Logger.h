/// Simple logger.
/// Adds log milliseconds and new line on the end of text.

#pragma once

#include <stdio.h>
#include "pico/stdlib.h"

#define LOG(str, ...) printf("%lld " str "\n", time_us_64()/1000, ##__VA_ARGS__);
