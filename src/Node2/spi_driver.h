/*!@file
* SPI interface
*/
#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

void spi_master_init();
char spi_master_transceive(char c);

#endif
