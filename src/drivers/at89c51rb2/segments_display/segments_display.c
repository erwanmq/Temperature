#include "drivers/at89c51rb2/segments_display/segments_display.h"
#include <8052.h>

#define ZERO    126
#define ONE     48
#define TWO     109
#define THREE   121
#define FOUR    51
#define FIVE    91
#define SIX     95
#define SEVEN   112
#define EIGHT   127
#define NINE    115

static const char g_digit_table[] = { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE };
static const unsigned char g_digit_table_size = sizeof(g_digit_table)/sizeof(g_digit_table[0]);

void segments_display_init(void)
{
    /* Turn off all port 2 */
    P2 = 0;
}

void segments_display_write_first_digit(char number)
{
    /* 0: -> A-B-C-D-E-F        : 0b01111110: 126 */
    /* 1: -> B-C                : 0b00110000: 48  */
    /* 2: -> A-B-D-E-G          : 0b01101101: 109 */
    /* 3: -> A-B-C-D-G          : 0b01111001: 121 */
    /* 4: -> B-C-F-G            : 0b00110011: 51  */
    /* 5: -> A-C-D-F-G          : 0b01011011: 91  */
    /* 6: -> A-C-D-E-F-G        : 0b01011111: 95  */
    /* 7: -> A-B-C              : 0b01110000: 112 */
    /* 8: -> A-B-C-D-E-F-G      : 0b01111111: 127 */
    /* 9: -> A-B-C-F-G          : 0b01110011: 115 */
    if (0 <= number && number < g_digit_table_size)
    {
        P0_1 = 1;
        P2 = g_digit_table[number];
        P0_0 = 0;
    }
}

void segments_display_write_second_digit(char number)
{
    if (0 <= number && number < g_digit_table_size)
    {
        P0_0 = 1;
        P2 = g_digit_table[number];
        P0_1 = 0;
    }
}
