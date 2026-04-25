#include <8052.h>
#include "timer/timer2.h"


__sfr __at (0xC3) SPCON;
__sfr __at (0xB1) IEN1; // Interrupt Enable Register
__sfr __at (0xC4) SPSTA;
__sfr __at (0xC5) SPDAT;



typedef enum SpiStatus
{
    STATUS_OK,
    STATUS_MODF,
    STATUS_WCOL,
    STATUS_OC,
    STATUS_SSERR,
    STATUS_NO_DATA,
} en_spi_status;

static volatile en_spi_status spi_status = STATUS_NO_DATA;
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
    timer2_init(100, 100);
    init_spi();

    send_spi_data();
    while(1)
    {
        spi_status = STATUS_OK;
        switch (spi_status)
        {
            case STATUS_OK:
                timer2_change_counter_max(1000);
                timer2_start_timer();
                spi_status = STATUS_NO_DATA;
                break;
        }
        if (timer2_get_flag())
        {
            P1_0 = !P1_0;
            timer2_reset_flag();
        }
    }
}

