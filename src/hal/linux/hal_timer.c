#include <time.h>
#include <stdio.h>
#include <errno.h>

#include "hal/hal_timer.h"

void hal_timer_init(void)
{
    // Do nothing
}

void hal_timer_sleep(uint16_t timeout_ms)
{

    time_t sec = timeout_ms / 1000;
    time_t nsec = (timeout_ms - sec * 1000) * 1000000;
    struct timespec request_sleep = { sec, nsec };
    errno = 0;
    if (0 != nanosleep(&request_sleep, NULL))
    {
        printf("nanosleep failed, errno == %d\n", errno);
    }
}

