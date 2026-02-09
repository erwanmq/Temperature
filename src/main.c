#include <8051.h>

/*
 * Simple delay loop
 * This is NOT accurate timing, just a busy wait
 */
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 120; j++)
        {
            __asm
                nop
            __endasm;
        }
    }
}

void main(void)
{
    // Configure P1.0 as output (8051 ports are quasi-bidirectional)
    P1_0 = 0;

    while (1)
    {
        P1_0 = 1;      // LED ON
        delay_ms(500);

        P1_0 = 0;      // LED OFF
        delay_ms(500);
    }
}

