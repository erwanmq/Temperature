
#include "hal/hal_timer.h"
#include "hal/hal_display.h"
#include "hal/hal_spi.h"
#include "application/FSM.h"

#ifdef TARGET_AT89C51RB2
// Needed to show interrupts functions next to the main function
#include "drivers/at89c51rb2/spi/spi.h"
#include "drivers/at89c51rb2/timer/timer2.h"
#include <8052.h> // Needed for EA bit
#endif


#define TIMER_FREQ 100 // 100Hz -- 10ms resolution

void main(void)
{
    /* Init drivers */
    hal_timer_init();
    hal_display_init();
    hal_spi_init();

    #ifdef TARGET_AT89C51RB2
    EA = 1; // For the moment, I have to find its place.
    #endif

    //external_interrupts_init_int1();
    //external_interrupts_init_int0();

    while(1)
    {
        FSM_update();
    }
}

