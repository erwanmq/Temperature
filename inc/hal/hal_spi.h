#ifndef HAL_SPI_H__
#define HAL_SPI_H__
#include <stdint.h>

void hal_spi_init(void);
uint8_t hal_spi_transaction(uint8_t data);
void hal_spi_acquire_bus(void);
void hal_spi_release_bus(void);

#endif // HAL_SPI_H__
