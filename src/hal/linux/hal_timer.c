#include <time.h>

#include "hal/hal_timer.h"

void hal_timer_init(void)
{
    // Do nothing
}

void hal_timer_sleep(uint16_t timeout_ms)
{
    nanosleep(timeout_ms * 1000);
}

