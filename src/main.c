#include <8052.h>
#include "drivers/timer/timer2.h"
#include "drivers/sleep/sleep.h"
#include "drivers/spi/spi.h"
#include "application/FSM.h"


#define TIMER_FREQ 100 // 100Hz -- 10ms resolution
#define TIMER_MAX_COUNTER 100 // 1/100 * 100 = 1000 ms

typedef enum State
{
    STATE_IDLE,
    STATE_TEMPERATURE,
    STATE_ERROR,
} en_state;

typedef enum Error
{
    ERROR_NO_ERR,
    ERROR_UNKNOWN,
    ERROR_SPI,
} en_error;

void main(void)
{
    EA = 1; // Enable interrupts
    timer2_init(TIMER_FREQ);
    spi_init();

    timer2_start_timer();
    unsigned char counter = 0;

    st_fsm_context FSM_ctx = {
        .current_state = FSM_STATE_IDLE,
    };

    //en_state current_state = STATE_IDLE;
    //en_spi_status spi_status = STATUS_OK;
    //en_error error = ERROR_NO_ERR;
    while(1)
    {
        FSM_update(&FSM_ctx);
       // switch (current_state)
       // {
       //     case STATE_IDLE:

       //         P1_0 = !P1_0;


       //         break;

       //     case STATE_ERROR:
       //         if (ERROR_SPI == error)
       //         {
       //             if (STATUS_MODF == spi_status)
       //             {
       //                // Do nothing
       //             }
       //         }
       //         break;

       //     case STATE_TEMPERATURE:
       //         /* 1 : start bit
       //          * 0: single ended - 1: differential input mode
       //          * 0: D0
       //          * 0: D1
       //          * 0: D2 -> All 0 == CH0
       //          */
       //         spi_enter_spi_mode();
       //         unsigned short temperature = 0xFFFF;
       //         for (int i = 2; i >= 0; i--)
       //         {
       //             spi_send_data(0b00000001);
       //             // Enter idle mode until next interrupt
       //             sleep_enter_idle_mode();

       //             __bit spi_flag = spi_get_flag();
       //             if (1 == spi_flag)
       //             {
       //                 spi_status = spi_get_status();
       //                 if (STATUS_OK != spi_status)
       //                 {
       //                     error = ERROR_SPI;
       //                     current_state = STATE_ERROR;
       //                 }
       //                 else
       //                 {
       //                     temperature = (spi_read_data() << (8 * i));
       //                 }
       //                 spi_reset_flag();
       //             }
       //         }

       //         spi_exit_spi_mode();

       //         temperature &= ~0xFC00; // Clear 6 first bits
       //         for (int i = 0; i < 16; i++)
       //         {
       //             P1_0 = (temperature >> i) & 1;
       //         }
       //         current_state = STATE_IDLE;

       //         break;
       // }

    }
}

