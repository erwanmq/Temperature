#include "drivers/at89c51rb2/segments_display/segments_display.h"

void hal_display_init(void)
{
    segments_display_init();
}

void hal_display_print_nb(int nb)
{
    int tens_digit = nb / 10;
    int unit_digit = nb - tens_digit * 10;
    segments_display_write_first_digit(tens_digit);
    segments_display_write_second_digit(unit_digit);
}

