#include <8052.h>

void timer2_isr(void) __interrupt(5)
{
    if (P1_0 == 1)
    {
        P1_0 &= ~0x01;
    }
    else
    {
        P1_0 |= 0x01;
    }
}

__sfr __at (0xC9) T2MOD;

void main(void)
{
    /* Configure timer 2 as auto-reload mode */
    T2MOD |= 0x01; // DCEN
    RCAP2H = 0x00;
    RCAP2L = 0xFF;

    TH2 = 0xFF;
    TL2 = 0xFF;

    /* Clear overflow/underflow bits */
    TF2 &= ~0x01;
    EXF2 &= ~0x01;

    /* Enable timer2 interrupt */
    EA = 1;
    ET2 |= 0x01;

    /* Enable the timer2 */
    TR2 |= 0x01;

    while(1)
    {}
}

