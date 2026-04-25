#ifndef TIMER2_H__
#define TIMER2_H__

void timer2_isr(void) __interrupt(5);
void timer2_init(unsigned int freq, unsigned int flag_counter);
volatile __bit timer2_get_flag(void);
void timer2_reset_flag(void);
void timer2_stop_timer(void);
void timer2_start_timer(void);
void timer2_change_counter_max(unsigned int flag_counter);
void timer2_change_freq(unsigned int freq);

#endif /* TIMER2_H__ */
