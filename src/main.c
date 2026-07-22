#include <8052.h>
#include "drivers/at89c51rb2/timer/timer2.h"
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
    timer2_init(TIMER_FREQ);
    spi_init();
    external_interrupts_init_int1();
    external_interrupts_init_int0();
    segments_display_init();

    timer2_start_timer();

    while(1)
    {
        FSM_update();
    }
}

