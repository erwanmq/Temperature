#ifndef HAL_TIMER_H__
#define HAL_TIMER_H__

#include <stdint.h>

void hal_timer_init(void);
void hal_timer_sleep(uint16_t timeout_ms);

#endif // HAL_TIMER_H__
