#include <8052.h>
#include "drivers/external_interrupts/external_interrupts.h"

static volatile __bit int1_flag = 0;
void int1_isr(void) __interrupt(2)
{
    int1_flag = 1;
}

void external_interrupts_init_int1(void)
{
    EX1 = 1;
}

__bit external_interrupts_get_int1_flag(void)
{
    return int1_flag;
}

void external_interrupts_reset_int1_flag(void)
{
    int1_flag = 0;
}

