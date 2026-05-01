#include <8052.h>

__sfr __at (0x87) PCON;
void sleep_enter_idle_mode(void)
{
    PCON = 0b00000001; // Set IDL bit to enter idle mode
    // Next instruction is paused.
}
