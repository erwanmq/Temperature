#include <8052.h>
#include "common/clock.h"
#include "drivers/timer/timer2.h"

#define PRESCALER_TIMER2        6U
#define CLK_TIMER2              CLK_PERIPH / PRESCALER_TIMER2

__sfr __at (0xC9) T2MOD;

static volatile __bit timer2_flag = 0;
void timer2_isr(void) __interrupt(5)
{
    TF2 = 0; // Clear the interrupt bit
    timer2_flag = 1;
}

void timer2_change_freq(unsigned int freq)
{
    unsigned int counter_overflow = 0xFFFF - (CLK_TIMER2 / freq);
    RCAP2H = (unsigned char)((counter_overflow & 0xFF00) >> 8);
    RCAP2L = (unsigned char)(counter_overflow & 0x00FF);
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

void timer2_init(unsigned int freq)
{
    /* Configure timer 2 as auto-reload mode */
    T2MOD |= 0x01; // DCEN
    
    timer2_change_freq(freq);

    TH2 = 0xFF;
    TL2 = 0xFF;

    /* Clear overflow/underflow bits */
    TF2 &= ~0x01;
    EXF2 &= ~0x01;

    /* Enable timer2 interrupt */
    ET2 |= 0x01;
}

__bit timer2_get_flag(void)
{
    return timer2_flag;
}

void timer2_reset_flag(void)
{
    timer2_flag = 0;
}
