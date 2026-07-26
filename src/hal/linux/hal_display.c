#include "hal/hal_display.h"

#include <stdio.h>
#include <time.h>

void hal_display_init(void)
{
    // Do nothing
}

void hal_display_print_nb(int nb)
{
    time_t raw_time;
    struct tm* timeinfo;

    time(&raw_time);
    timeinfo = localtime(&raw_time);
    printf("%s -- %i\n", asctime(timeinfo), nb);
}
