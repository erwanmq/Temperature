#include <8052.h>
#include "drivers/timer/timer2.h"
#include "drivers/sleep/sleep.h"
#include "drivers/spi/spi.h"
#include "drivers/external_interrupts/external_interrupts.h"
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

    timer2_start_timer();

    while(1)
    {
        FSM_update();
    }
}

