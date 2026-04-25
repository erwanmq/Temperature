#include <8052.h>

__sfr __at (0xC9) T2MOD;

__sfr __at (0xC3) SPCON;
__sfr __at (0xB1) IEN1; // Interrupt Enable Register
__sfr __at (0xC4) SPSTA;
__sfr __at (0xC5) SPDAT;


static volatile unsigned int counter = 0;
void timer2_isr(void) __interrupt(5)
{
    TR2 &= ~0x01;

    counter++;
    TF2 &= ~0x01;

    TR2 |= 0x01;
}

typedef enum SpiStatus
{
    STATUS_OK,
    STATUS_MODF,
    STATUS_WCOL,
    STATUS_OC,
    STATUS_SSERR,
} en_spi_status;

//static volatile en_spi_status spi_status = STATUS_OK;
//void spi_isr(void) __interrupt(9)
//{
//    unsigned char spsta = SPSTA;
//
//    if (spsta & 0x80)
//    {
//        spi_status = STATUS_OK;
//    }
//
//    if (spsta & 0x40)
//    {
//        spi_status = STATUS_WCOL;
//    }
//
//    if (spsta & 0x20)
//    {
//        spi_status = STATUS_SSERR;
//    }
//
//    if (spsta & 0x10)
//    {
//       spi_status = STATUS_MODF; 
//    }
//}


void init_timer2(void)
{
    /* Configure timer 2 as auto-reload mode */
    T2MOD |= 0x01; // DCEN
    RCAP2H = 0x5D;
    RCAP2L = 0x3C;

    TH2 = 0xFF;
    TL2 = 0xFF;

    /* Clear overflow/underflow bits */
    TF2 &= ~0x01;
    EXF2 &= ~0x01;

    /* Enable timer2 interrupt */
    ET2 |= 0x01;
    
    /* Enable the timer2 */
    TR2 |= 0x01;
}

void init_spi(void)
{
//    IEN1 |= 0x04; // Set SPI interrupt
    // Master mode, FCLK/2, Mode 0
    SPCON = 0x30; 
    SPCON |= 0x40; // Enable SPI
}

void send_spi_data(void)
{
    //volatile int spsta = SPSTA;
    //volatile int spdat = SPDAT;
    //while (!(SPSTA & 0x80)); // Wait for SPIF flag
    //{
    //    spsta = SPSTA;
    //    spdat = SPDAT;
    //}
    SPDAT = 0b10010110;
}

void main(void)
{
    EA = 1; // Enable interrupts
    init_timer2();
    init_spi();

    while(1)
    {
        if (counter >= 100)
        {
            if (P1_0 == 1)
            {
                P1_0 &= ~0x01;
                send_spi_data();
            }
            else
            {
                P1_0 |= 0x01;
            }
            counter = 0;
        }
    }
}

