#include "application/FSM.h"

#include "utils/stack.h"

#include "drivers/timer/timer2.h"
#include "drivers/sleep/sleep.h"
#include "drivers/spi/spi.h"

#define TIMER_BEFORE_FETCH 100 // 1/100 * 100 = 1000 ms

static void (*fsm_callback[])(st_fsm_context*) = {
    [FSM_STATE_IDLE] = fsm_state_idle,
    [FSM_STATE_ADC_SAMPLE] = fsm_state_adc_sample,
    [FSM_STATE_CALCULATE_MEAN] = fsm_state_calculate_mean,
    [FSM_STATE_DISPLAY] = fsm_state_display,
};

static void fsm_state_idle(st_fsm_context* ctx)
{
    /* Enter in Sleep mode and wait for next interrupt */
    timer2_reset_flag();
    timer2_start_timer();

    // Enter idle mode until next interrupt
    sleep_enter_idle_mode();
    // The CPU sleep here until next interrupt

    __bit timer2_flag = timer2_get_flag();
    if (1 == timer2_flag)
    {
        ctx->counter++;
        if (ctx->counter >= ctx->max_counter)
        {
            ctx->current_state = FSM_STATE_ADC_SAMPLE;
            ctx->counter = 0;
            timer2_stop_timer();
        }
    }
}

static void fsm_state_adc_sample(st_fsm_context* ctx)
{
    spi_set_SS_low();
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
        else
        {
            ctx->current_state = FSM_STATE_IDLE; // Go to idle state to wait next fetching
            ctx->max_counter = timer2_ms_to_counter(5000); // 5 seconds

        }
    }
    else if (0 == ctx->spi_performed)
    {
        spi_send_data(0b00000001);
        ctx->nb_adc_comm++;
        ctx->spi_performed = 1;
    }

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
        spi_reset_flag();
        ctx->spi_performed = 0; // Reset flag
    }

}

static void fsm_state_calculate_mean(st_fsm_context* ctx)
{
    unsigned short mean_sample = 0; // Short - 2 bytes to contains the addition of 4 char - 1 byte
    for (int i = 0; i < sizeof(ctx->adc_samples); i++)
    {
        mean_sample += ctx->adc_samples[i];
    }

    mean_sample /= 4;

    /* Send the value to SPI */
    spi_send_data(mean_sample);

    ctx->current_state = FSM_STATE_IDLE;
    ctx->max_counter = timer2_ms_to_counter(10000); // 10 seconds
}

static void fsm_state_display(st_fsm_context* ctx)
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
