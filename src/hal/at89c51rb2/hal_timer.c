#include "drivers/at89c51rb2/timer/timer2.h"
#include "drivers/at89c51rb2/sleep/sleep.h"
#include "hal/hal_timer.h"
#include <8052.h>

#define TIMER_FREQ 100 // 100Hz -- 10ms resolution

void hal_timer_init(void)
{
    timer2_init(TIMER_FREQ);
}

void hal_timer_sleep(uint16_t timeout_ms)
{
    uint16_t counter = 0;

    const uint16_t counter_wanted = (uint16_t)(timer2_ms_to_counter(timeout_ms));
    timer2_start_timer();

    while (counter < counter_wanted)
    {
        sleep_enter_idle_mode();

        __bit timer2_flag = timer2_get_flag();
        if (1 == timer2_flag)
        {
            timer2_reset_flag();
            counter++;
        }
        timer2_reset_flag();
    }

    timer2_stop_timer();
}

