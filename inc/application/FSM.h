#ifndef FSM_H__
#define FSM_H__

typedef enum FSM_State
{
    FSM_STATE_IDLE,
    FSM_STATE_ADC_SAMPLE,
    FSM_STATE_CALCULATE_MEAN,
    FSM_STATE_DISPLAY,
    FSM_STATE_MAX_COUNT__,
} en_fsm_state;

typedef struct FSM_Context
{
    en_fsm_state current_state;

    /* IDLE context variables */
    unsigned char counter;

    /* ADC context variables */
    unsigned char nb_adc_comm;
    unsigned short adc_value;
    unsigned char nb_acq;

    /* Calculate mean context variables */
    unsigned char adc_samples[4];
} st_fsm_context;


void FSM_update(st_fsm_context* ctx);

void fsm_state_idle(st_fsm_context* ctx);
void fsm_state_adc_sample(st_fsm_context* ctx);
void fsm_state_calculate_mean(st_fsm_context* ctx);
void fsm_state_display(st_fsm_context* ctx);


#endif
