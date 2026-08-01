#include "drivers/at89c51rb2/mcp3008/mcp3008.h"

#include "hal/hal_adc.h"

uint16_t adc_read_value(void)
{
    return mcp3008_read_value();
}
