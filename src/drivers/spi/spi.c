#include <8052.h>
#include "common/clock.h"
#include "drivers/spi/spi.h"


#define CLK_SPI CLK_PERIPH

__sfr __at (0xC3) SPCON;
__sfr __at (0xB1) IEN1; // Interrupt Enable Register
__sfr __at (0xC4) SPSTA;
__sfr __at (0xC5) SPDAT;

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


void spi_init(void)
{
    // Master mode, FCLK/2, Mode 0
    SPCON = 0x30; 
    
    IEN1 |= 0x04; // Set SPI interrupt
    SPCON |= 0x40; // Enable SPI
}

void spi_send_data(unsigned char data)
{
    SPDAT = data;
}

unsigned char spi_read_data(void)
{
    return SPDAT;
}

en_spi_status spi_get_status(void)
{
    return spi_status;
}

