#include "drivers/at89c51rb2/mcp3008/mcp3008.h"

#include "hal/hal_spi.h"


uint8_t mcp3008_read_value(void)
{
    uint8_t nb_fetch = 0;
    uint8_t value_retrieved = 0;
    hal_spi_acquire_bus();

    while (nb_fetch < 3)
    {
        value_retrieved |= (hal_spi_transaction(0b00000001) << (8 * (nb_fetch - 1)));
        nb_fetch++;
    }
    hal_spi_release_bus();
    return value_retrieved;
}
