#ifndef OLED_H
#define OLED_H
#define F_CPU 4915200
#define ASCII_OFFSET ((volatile unsigned int) 32)

//#define oled_command_address ((volatile char*)0x1000)
//#define oled_data_address ((volatile char*)0x1200)

#include <stdint.h>


void oled_init();
void oled_write_c(uint8_t command);
void oled_write_d(uint8_t data);
void oled_print_char(unsigned char c);
void oled_reset(void);  //Funker ikke helt
void oled_print_string(char* str);  //Funker
void oled_goto_line(int line);  //Funker
void oled_goto_column(int column);  //Funker
void oled_home(void);   //Funker
void oled_clear_line(int line); //Funker
void oled_pos(int row,int column);
#endif
