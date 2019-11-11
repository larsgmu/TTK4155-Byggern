/*!@file
* This file contains functions to use the IDT_7164SL chip as external SRAM for the atmega162.
*/
#ifndef SRAM_H
#define SRAM_H

#include <stdlib.h>
#include <stdint.h>

/*!
*@brief Enables the external memory interface, and releases PC7 - PC4
*/
void          SRAM_init();

/*!
*@brief Writes specified data to given address.
*@param[in] @c unsigned int adr -> Address to write data to.
*@param[in] @c usigned int data -> Data to write to address.
*/
void          SRAM_write(unsigned int adr, unsigned int data);

/*!
*@brief Read data from specified address.
*@param[in] @c unsigned int adr -> Address to read from.
*@return @c usigned int -> Data read from address.
*/
unsigned int  SRAM_read(unsigned int adr);

/*!
*@brief SRAM test.
*/
void          SRAM_test(void);

#endif
