#include "hal/hal_adc.h"

uint16_t adc_read_value(void)
{
    static uint16_t random_value = 23;
    return random_value++;
}
