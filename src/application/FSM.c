#include "application/FSM.h"

#include "utils/stack.h"

#include "drivers/timer/timer2.h"
#include "drivers/sleep/sleep.h"
#include "drivers/spi/spi.h"

#define TIMER_MAX_COUNTER 100 // 1/100 * 100 = 1000 ms

void (*fsm_callback[])(st_fsm_context*) = {
    [FSM_STATE_IDLE] = fsm_state_idle,
    [FSM_STATE_ADC_SAMPLE] = fsm_state_adc_sample,
    [FSM_STATE_CALCULATE_MEAN] = fsm_state_calculate_mean,
    [FSM_STATE_DISPLAY] = fsm_state_display,
};

/* TODO: Maybe pass to the context a variable for the task to know where it should jump.
 * Because this task is only a kind of 'wait' state.
 * Or it's like a control task that will dispatch the next state based on interrupts.
 */
void fsm_state_idle(st_fsm_context* ctx)
{
    /* Enter in Sleep mode and wait for next interrupt */
    timer2_reset_flag();
    // Enter idle mode until next interrupt
    sleep_enter_idle_mode();
    // The CPU sleep here until next interrupt

    __bit timer2_flag = timer2_get_flag();
    if (1 == timer2_flag)
    {
        ctx->counter++;
        if (ctx->counter >= TIMER_MAX_COUNTER)
        {
            ctx->current_state = FSM_STATE_ADC_SAMPLE;
            ctx->counter = 0;
        }
    }
    /* TODO: Check if its spi flag to enter adc_sample directly */
    else
    {
        ctx->current_state = FSM_STATE_ADC_SAMPLE;
    }
}

void fsm_state_adc_sample(st_fsm_context* ctx)
{
    //spi_set_SS_low();
    __bit spi_flag = spi_get_flag();
    en_spi_status spi_status = STATUS_OK;
    if (1 == spi_flag)
    {
        spi_status = spi_get_status();
        if (STATUS_OK != spi_status)
        {
            // Process error
        }
        else
        {
            ctx->adc_value |= (spi_read_data() << (8 * (ctx->nb_adc_comm - 1)));
        }
        //spi_reset_flag();
    }

    /* We need 3 SPI communication to retrieve all the ADC data */
    if (ctx->nb_adc_comm == 3)
    {
        spi_set_SS_high();
        ctx->nb_adc_comm = 0;

        ctx->adc_samples[ctx->nb_acq] = ctx->adc_value;
        ctx->adc_value = 0;
        ctx->nb_acq++;

        if (3 == ctx->nb_acq)
        {
            ctx->current_state = FSM_STATE_CALCULATE_MEAN;
            ctx->nb_acq = 0;
        }
    }
    else
    {
        //spi_send_data(0b00000001);
        ctx->nb_adc_comm++;

        /* Go to IDLE state while waiting for the answer */
        ctx->current_state = FSM_STATE_IDLE;
    }
}

void fsm_state_calculate_mean(st_fsm_context* ctx)
{
    unsigned short mean_sample = 0; // Short - 2 bytes to contains the addition of 4 char - 1 byte
    for (int i = 0; i < sizeof(ctx->adc_samples); i++)
    {
        mean_sample += ctx->adc_samples[i];
    }

    mean_sample /= 4;

    /* Send the value to SPI */
    //spi_send_data(mean_sample);

    unsigned char stack_rem = stack_remaining();
    spi_send_data(stack_rem);

    ctx->current_state = FSM_STATE_IDLE;
}

void fsm_state_display(st_fsm_context* ctx)
{
    // Do nothing
}

void FSM_update(st_fsm_context* ctx)
{
    if (ctx->current_state >= FSM_STATE_MAX_COUNT__)
    {
        /* Invalid state */
        return;
    }
    fsm_callback[ctx->current_state](ctx);
}
