#include <8052.h>

__sfr __at (0x87) PCON;
void sleep_enter_idle_mode(void)
{
    PCON |= 0b00000001; // Set IDL bit to enter idle mode.
    // Next instruction is paused.
    // It can be wake up by any interrupts.
}

void sleep_enter_power_down_mode(void)
{
    PCON |= 0b00000010; // Set PD bit to enter power down mode.
    // Next instruction is paused.
    // It can be wake up only by INT0 or INT1 interrupt.
}
