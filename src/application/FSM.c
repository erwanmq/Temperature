#include "application/FSM.h"

#include "utils/stack.h"

#include "hal/hal_timer.h"
#include "hal/hal_spi.h"
#include "hal/hal_display.h"
#include "hal/hal_adc.h"

#define TIMER_BEFORE_FETCH 100 // 1/100 * 100 = 1000 ms

static st_fsm_context ctx = {
    .current_state = FSM_STATE_ADC_SAMPLE,
};

static void fsm_transition_wait(unsigned int timeout_ms, en_fsm_state next_state)
{
    ctx.wait_timeout = timeout_ms;
    ctx.next_state = next_state;
    ctx.current_state = FSM_STATE_WAIT;
}

static void fsm_state_wait(void)
{
    hal_timer_sleep(ctx.wait_timeout);
    ctx.current_state = ctx.next_state;
}

static void fsm_state_adc_sample(void)
{
    ctx.adc_samples[ctx.nb_acq] = adc_read_value();
    ctx.nb_acq++;

    if (ctx.nb_acq > 3)
    {
        ctx.current_state = FSM_STATE_CALCULATE_MEAN;
        ctx.nb_acq = 0;
    }
    else
    {
        fsm_transition_wait(5000, FSM_STATE_ADC_SAMPLE);
    }
}

static void fsm_state_calculate_mean(void)
{
    unsigned short mean_sample = 0; // Short - 2 bytes to contains the addition of 4 char - 1 byte
    for (int i = 0; i < sizeof(ctx.adc_samples)/sizeof(ctx.adc_samples[0]); i++)
    {
        mean_sample += ctx.adc_samples[i];
    }

    mean_sample /= 4;

    ctx.mean_sample = (unsigned char)mean_sample;

    /* Send the value to SPI */
    hal_spi_transaction(mean_sample);

    fsm_transition_wait(0, FSM_STATE_DISPLAY);
}

static void fsm_state_display(void)
{
    hal_display_print_nb(ctx.mean_sample);
    fsm_transition_wait(1000, FSM_STATE_ADC_SAMPLE);
}

static void (*fsm_callback[])(void) = {
    [FSM_STATE_WAIT] = fsm_state_wait,
    [FSM_STATE_ADC_SAMPLE] = fsm_state_adc_sample,
    [FSM_STATE_CALCULATE_MEAN] = fsm_state_calculate_mean,
    [FSM_STATE_DISPLAY] = fsm_state_display,
};

void FSM_update(void)
{
    if (ctx.current_state >= FSM_STATE_MAX_COUNT__)
    {
        /* Invalid state */
        return;
    }
    fsm_callback[ctx.current_state]();

    ///* Used by short press */
    //__bit int1_flag = external_interrupts_get_int1_flag();
    //if (1 == int1_flag)
    //{
    //    ctx.current_state = FSM_STATE_DISPLAY;
    //    external_interrupts_reset_int1_flag();
    //}

    ///* Used by long press */
    //__bit int0_flag = external_interrupts_get_int0_flag();
    //if (1 == int0_flag)
    //{
    //    external_interrupts_reset_int0_flag();
    //    sleep_enter_power_down_mode();
    //}
}
