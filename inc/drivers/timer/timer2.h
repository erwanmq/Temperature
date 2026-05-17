#ifndef TIMER2_H__
#define TIMER2_H__

void timer2_isr(void) __interrupt(5);
void timer2_init(unsigned int freq);
void timer2_stop_timer(void);
void timer2_start_timer(void);
void timer2_change_freq(unsigned int freq);
__bit timer2_get_flag(void);
void timer2_reset_flag(void);
unsigned int timer2_ms_to_counter(unsigned int ms);


#endif /* TIMER2_H__ */
