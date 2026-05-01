#include <8052.h>
#include "drivers/timer/timer2.h"
#include "drivers/sleep/sleep.h"
#include "drivers/spi/spi.h"


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

    en_state current_state = STATE_IDLE;
    en_spi_status spi_status = STATUS_OK;
    en_error error = ERROR_NO_ERR;
    while(1)
    {
        switch (current_state)
        {
            case STATE_IDLE:
                timer2_reset_flag();

                // Enter idle mode until next interrupt
                sleep_enter_idle_mode();
                // The CPU sleep here until next interrupt
                P1_0 = !P1_0;


                __bit timer2_flag = timer2_get_flag();
                if (1 == timer2_flag)
                {
                    counter++;
                    if (counter >= TIMER_MAX_COUNTER)
                    {
                        current_state = STATE_TEMPERATURE;      
                        counter = 0;
                    }
                }
                else 
                {
                }

                break;
            case STATE_ERROR:
                if (ERROR_SPI == error)
                {
                    if (STATUS_MODF == spi_status)
                    {
                       // Do nothing 
                    }
                }
                break;

            case STATE_TEMPERATURE:
                spi_send_data(0x55);
                
                // Enter idle mode until next interrupt
                sleep_enter_idle_mode();

                __bit spi_flag = spi_get_flag();
                if (1 == spi_flag)
                {
                    spi_status = spi_get_status();
                    if (STATUS_OK != spi_status)
                    {
                        error = ERROR_SPI;
                        current_state = STATE_ERROR;
                    }
                    else 
                    {
                        unsigned char temp = spi_read_data(); // Don't use it
                    }
                    spi_reset_flag();
                }
                else
                {
                }

                break;
        }
        
    }
}

