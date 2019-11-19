/*!@file
* This file contains functions to enable SPI communication between the atmega162 and MCP2515 chip.
*/
#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

/*!
*@brief Configures the atmega162 pins and enables SPI
*/
void spi_master_init();

/*!
*@brief Function to transmit and receive data.
*@param[in] @c char c -> Charachter to transmit.
*@return @c char -> Charachter received.
*/
char spi_master_transceive(char c);

#endif
