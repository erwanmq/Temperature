#ifndef EXTERNAL_INTERRUPTS_H__
#define EXTERNAL_INTERRUPTS_H__
void int0_isr(void) __interrupt(0);
void int1_isr(void) __interrupt(2);

void external_interrupts_init_int0(void);
__bit external_interrupts_get_int0_flag(void);
void external_interrupts_reset_int0_flag(void);

void external_interrupts_init_int1(void);
__bit external_interrupts_get_int1_flag(void);
void external_interrupts_reset_int1_flag(void);

#endif /* EXTERNAL_INTERRUPTS_H__ */
