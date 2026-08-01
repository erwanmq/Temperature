
#include "hal/hal_timer.h"
#include "hal/hal_display.h"
#include "hal/hal_spi.h"
#include "application/FSM.h"

#include "drivers/at89c51rb2/spi/spi.h"
#include "drivers/at89c51rb2/timer/timer2.h"

#include <8052.h>


#define TIMER_FREQ 100 // 100Hz -- 10ms resolution

void main(void)
{
    /* Init drivers */
    hal_timer_init();
    hal_display_init();
    hal_spi_init();
    EA = 1; // For the moment, I have to find its place.
    P1_0 = 0;

    //external_interrupts_init_int1();
    //external_interrupts_init_int0();

    while(1)
    {
        FSM_update();
    }
}

