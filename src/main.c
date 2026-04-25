#include <8052.h>
#include "drivers/timer/timer2.h"
#include "drivers/spi/spi.h"

//void send_spi_data(void)
//{
//    //volatile int spsta = SPSTA;
//    //volatile int spdat = SPDAT;
//    //while (!(SPSTA & 0x80)); // Wait for SPIF flag
//    //{
//    //    spsta = SPSTA;
//    //    spdat = SPDAT;
//    //}
//}

void main(void)
{
    EA = 1; // Enable interrupts
    timer2_init(100, 100);
    spi_init();

    spi_send_data(0x55);
    while(1)
    {
        en_spi_status spi_status = spi_get_status();
        switch (spi_status)
        {
            case STATUS_OK:
                timer2_change_counter_max(1000);
                timer2_start_timer();
                spi_status = STATUS_NO_DATA;
                break;
        }
        if (timer2_get_flag())
        {
            P1_0 = !P1_0;
            timer2_reset_flag();
        }
    }
}

