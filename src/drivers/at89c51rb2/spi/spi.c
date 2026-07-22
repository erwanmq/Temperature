#include <8052.h>
#include "common/clock.h"
#include "drivers/at89c51rb2/spi/spi.h"


#define CLK_SPI CLK_PERIPH

__sfr __at (0xC3) SPCON;
__sfr __at (0xB1) IEN1; // Interrupt Enable Register
__sfr __at (0xC4) SPSTA;
__sfr __at (0xC5) SPDAT;

static volatile __bit spi_flag = 0;
static volatile __bit active_flag = 0;
void spi_isr(void) __interrupt(9)
{
    spi_flag = 1;
}


void spi_init(void)
{
    // Master mode, FCLK/2, Mode 0
    SPCON = 0x30; 

    IEN1 |= 0x04; // Set SPI interrupt
    SPCON |= 0x40; // Enable SPI
}

__bit spi_get_flag(void)
{
    return spi_flag;
}

void spi_reset_flag(void)
{
    spi_flag = 0;
}

void spi_send_data(unsigned char data)
{
    SPDAT = data;
}

void spi_set_SS_low(void)
{
    // Make SS low
    P1_2 = 0;
    active_flag = 1;
}

void spi_set_SS_high(void)
{
    // Make SS high
    P1_2 = 1;
    active_flag = 0;
}

unsigned char spi_read_data(void)
{
    return SPDAT;
}

en_spi_status spi_get_status(void)
{
    en_spi_status spi_status = STATUS_NO_DATA;
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
    return spi_status;
}

