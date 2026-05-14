#include <8052.h>

#define STACK_TOP    0x7F

unsigned char stack_remaining(void)
{
    if (SP >= STACK_TOP)
        return 0;

    return STACK_TOP - SP;
}

