#ifndef OLED_H
#define OLED_H
#define F_CPU 4915200
#define ASCII_OFFSET ((volatile unsigned int) 32)

//#define oled_command_address ((volatile char*)0x1000)
//#define oled_data_address ((volatile char*)0x1200)

#include <stdint.h>


void oled_init();
void oled_write_c(unsigned char out_command);
void oled_write_d(unsigned char out_data);
void oled_print_char(unsigned char c);
void OLED_print(unsigned char c);
void oled_go_to_page(uint8_t page);
void oled_top_left(void);

void oled_reset(void);


#endif
