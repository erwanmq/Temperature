#include <8052.h>

__sfr __at (0xC9) T2MOD;

__sfr __at (0xC3) SPCON;
__bit __at (0xB3) ESPI;
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

static volatile en_spi_status spi_status = STATUS_OK;
void spi_isr(void) __interrupt(9)
{
    unsigned char spsta = SPSTA;

    if (spsta & 0x80)
    {
        spi_status = STATUS_OK;
    }

    if (spsta & 0x40)
    {
        spi_status = STATUS_WCOL;
    }

    if (spsta & 0x20)
    {
        spi_status = STATUS_SSERR;
    }

    if (spsta & 0x10)
    {
       spi_status = STATUS_MODF; 
    }
}


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
    /* Default to :
     * FCLK / 2
     * MASTER
     * SS Enabled
     * CPOL - CPHA to 0
     * */
    ESPI= 1; // Set SPI interrupt
    SPCON |= 0x40; // Enable SPI interface
}

void send_spi_data(void)
{
    SPDAT = 0b10010110;
}

void main(void)
{
    EA = 1; // Enable interrupts
    init_timer2();
    //init_spi();
    //send_spi_data();

    while(1)
    {
        if (counter >= 100)
        {
            if (P1_0 == 1)
            {
                P1_0 &= ~0x01;
            }
            else
            {
                P1_0 |= 0x01;
            }
            counter = 0;
        }
    }
}

