#include "drivers/at89c51rb2/timer/timer2.h"
#include "drivers/at89c51rb2/sleep/sleep.h"
#include "hal/hal_timer.h"

#define TIMER_FREQ 100 // 100Hz -- 10ms resolution

void hal_timer_init(void)
{
    EA = 1; // For the moment, I have to find its place.
    timer2_init(TIMER_FREQ);
}

void hal_timer_sleep(uint16_t timeout_ms)
{
    uint16_t counter = 0;

    const uint16_t counter_wanted = (uint16_t)(timer2_ms_to_counter(timeout_ms));
    timer2_start_timer();

    // Avoid blocking indefinetely here
    while (0 == counter)
    {
        sleep_enter_idle_mode();

        __bit timer2_flag = timer2_get_flag();
        if (1 == timer2_flag)
        {
            timer2_reset_flag();
            counter++;
            if (counter == counter_wanted)
            {
                counter = 0;
            }
        }
        timer2_reset_flag();
    }

    timer2_stop_timer();
}

