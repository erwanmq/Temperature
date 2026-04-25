#include <8052.h>
#include "common/clock.h"

#define PRESCALER_TIMER2        6U
#define CLK_TIMER2              CLK_PERIPH / PRESCALER_TIMER2

__sfr __at (0xC9) T2MOD;

volatile unsigned int counter_limit = 1;
volatile __bit flag = 0;
void timer2_isr(void) __interrupt(5)
{
    static volatile unsigned int counter = 0;
    TF2 &= ~0x01; // Clear the interrupt bit

    counter++;

    if (counter >= counter_limit)
    {
        counter = 0;
        flag = 1;
    }

}

void timer2_change_freq(unsigned int freq)
{
    unsigned int counter_overflow = 0xFFFF - (CLK_TIMER2 / freq);
    RCAP2H = (unsigned char)((counter_overflow & 0xFF00) >> 8);
    RCAP2L = (unsigned char)(counter_overflow & 0x00FF);
}

void timer2_change_counter_max(unsigned int flag_counter)
{
    counter_limit = flag_counter;
}

void timer2_start_timer(void)
{
    /* Enable the timer2 */
    TR2 |= 0x01;
}

void timer2_stop_timer(void)
{
    /* Disable the timer2 */
    TR2 &= ~0x01;
}

void timer2_init(unsigned int freq, unsigned int flag_counter)
{
    /* Configure timer 2 as auto-reload mode */
    T2MOD |= 0x01; // DCEN
    
    timer2_change_freq(freq);
    timer2_change_counter_max(flag_counter);

    TH2 = 0xFF;
    TL2 = 0xFF;

    /* Clear overflow/underflow bits */
    TF2 &= ~0x01;
    EXF2 &= ~0x01;

    /* Enable timer2 interrupt */
    ET2 |= 0x01;
}


volatile __bit timer2_get_flag(void)
{
    return flag;
}

void timer2_reset_flag(void)
{
    flag = 0;
}
