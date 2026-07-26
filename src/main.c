
#include "hal/hal_timer.h"
#include "hal/hal_display.h"
#include "application/FSM.h"


#define TIMER_FREQ 100 // 100Hz -- 10ms resolution

void main(void)
{
    /* Init drivers */
    hal_timer_init();
    hal_display_init();
    //external_interrupts_init_int1();
    //external_interrupts_init_int0();

    while(1)
    {
        FSM_update();
    }
}

