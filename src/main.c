#include <8052.h>

#include "hal/hal_timer.h"
#include "drivers/at89c51rb2/sleep/sleep.h"
#include "drivers/at89c51rb2/spi/spi.h"
#include "drivers/at89c51rb2/external_interrupts/external_interrupts.h"
#include "drivers/at89c51rb2/segments_display/segments_display.h"
#include "application/FSM.h"
#include "utils/stack.h"


#define TIMER_FREQ 100 // 100Hz -- 10ms resolution

void main(void)
{
    EA = 1; // Enable interrupts

    /* Init drivers */
    hal_timer_init();
    hal_display_init();
    external_interrupts_init_int1();
    external_interrupts_init_int0();

    while(1)
    {
        FSM_update();
    }
}

