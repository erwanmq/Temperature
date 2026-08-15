#ifdef TARGET_AT89C51RB2
#include <8052.h>
#define GPIO_DEBUG_TOGGLE() (P1_0 = !P1_0)
#else
#define GPIO_DEBUG_TOGGLE() ((void)0)
#endif
