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
void sram_init();

/*!
*@brief SRAM test.
*/
void sram_test();

#endif
