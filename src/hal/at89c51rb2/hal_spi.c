#include "drivers/at89c51rb2/spi/spi.h"

#include "hal/hal_spi.h"

void hal_spi_init(void)
{
    spi_init();
}

uint8_t hal_spi_transaction(uint8_t data)
{
    uint8_t data_received = 0;
    spi_send_data(data);

    __bit spi_flag = 0;
    do
    {
        spi_flag = spi_get_flag();
    } while (spi_flag != 1);

    en_spi_status spi_status = STATUS_OK;
    if (1 == spi_flag)
    {
        spi_status = spi_get_status();
        if (STATUS_OK != spi_status)
        {
            // Process error
        }
        else
        {
            data_received = spi_read_data();
        }
    }
    spi_reset_flag();
}

void hal_spi_acquire_bus(void)
{
    spi_set_SS_low();
}

void hal_spi_release_bus(void)
{
    spi_set_SS_high();
}

