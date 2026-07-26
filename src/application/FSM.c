#include "application/FSM.h"

#include "utils/stack.h"

#include "hal/hal_timer.h"
#include "hal/hal_spi.h"
#include "hal/hal_display.h"

#define TIMER_BEFORE_FETCH 100 // 1/100 * 100 = 1000 ms

static void (*fsm_callback[])(void) = {
    [FSM_STATE_WAIT] = fsm_state_wait,
    [FSM_STATE_ADC_SAMPLE] = fsm_state_adc_sample,
    [FSM_STATE_CALCULATE_MEAN] = fsm_state_calculate_mean,
    [FSM_STATE_DISPLAY] = fsm_state_display,
};

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
    hal_spi_acquire_bus();
    /* We need 3 SPI communication to retrieve all the ADC data */
    if (ctx.nb_adc_comm == 3)
    {
        hal_spi_release_bus();
        ctx.nb_adc_comm = 0;

        ctx.adc_samples[ctx.nb_acq] = ctx.adc_value;
        ctx.adc_value = 0;
        ctx.nb_acq++;

        if (3 == ctx.nb_acq)
        {
            ctx.current_state = FSM_STATE_CALCULATE_MEAN;
            ctx.nb_acq = 0;
        }
        else
        {
            fsm_transition_wait(5000, FSM_STATE_ADC_SAMPLE);
        }
    }
    else
    {
        ctx.adc_value |= (hal_spi_transaction(0b00000001) << (8 * (ctx.nb_adc_comm - 1)));
        ctx.nb_adc_comm++;
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

    fsm_transition_wait(10000, FSM_STATE_ADC_SAMPLE);
}

static void fsm_state_display(void)
{
    hal_display_print_nb(23);
}

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
