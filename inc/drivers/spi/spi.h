#ifndef SPI_H__
#define SPI_H__

typedef enum SpiStatus
{
    STATUS_OK,
    STATUS_MODF,
    STATUS_WCOL,
    STATUS_OC,
    STATUS_SSERR,
    STATUS_NO_DATA,
} en_spi_status;


void spi_isr(void) __interrupt(9);
void spi_init(void);
void spi_reset_flag(void);
__bit spi_get_flag(void);
void spi_send_data(unsigned char data);
unsigned char spi_read_data(void);
en_spi_status spi_get_status(void);
void spi_exit_spi_mode(void);
void spi_enter_spi_mode(void);

#endif /* SPI_H__ */
