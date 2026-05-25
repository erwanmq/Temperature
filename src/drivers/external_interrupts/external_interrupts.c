#include <8052.h>
#include "drivers/external_interrupts/external_interrupts.h"


static volatile __bit int0_flag = 0;
void int0_isr(void) __interrupt(0)
{
    int0_flag = 1;
}

void external_interrupts_init_int0(void)
{
    EX0 = 1;
}

__bit external_interrupts_get_int0_flag(void)
{
    return int0_flag;
}

void external_interrupts_reset_int0_flag(void)
{
    int0_flag = 0;
}

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

