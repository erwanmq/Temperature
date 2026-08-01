#include "drivers/at89c51rb2/mcp3008/mcp3008.h"

#include "hal/hal_spi.h"

#include <8052.h>


uint16_t mcp3008_read_value(void)
{
    uint8_t nb_fetch = 0;
    uint8_t value_retrieved[3];
    hal_spi_acquire_bus();

    while (nb_fetch < 3)
    {
        value_retrieved[nb_fetch] = hal_spi_transaction(0b00000001);
        nb_fetch++;
    }
    hal_spi_release_bus();

    uint16_t value_wanted = value_retrieved[2] | ((value_retrieved[1] & 0x03) << 8);
    return value_wanted;
}
