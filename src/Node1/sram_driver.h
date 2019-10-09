/*!@file
* SRAM interface
*/
#ifndef SRAM_H
#define SRAM_H

#include <stdlib.h>
#include <stdint.h>

void SRAM_init();
void SRAM_write(unsigned int adr, unsigned int data);
unsigned int SRAM_read(unsigned int adr);
void SRAM_test(void);

#endif
