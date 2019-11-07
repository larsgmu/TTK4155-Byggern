/*!@file
* OLED interface
*/
#ifndef OLED_H
#define OLED_H

#define OLED_ROWS           64
#define OLED_COLS           128
#define OLED_PAGES          8
#define OLED_PAGE_HEIGHT    8

#define F_CPU 4915200
#define ASCII_OFFSET ((volatile unsigned int) 32)

#include <stdint.h>
#include "menu.h"

/*Initialize OLED screen*/
void oled_init();
/*Writes char to OLED*/
void oled_write_c(uint8_t command);
void oled_write_d(uint8_t data);
void oled_goto_line(int line);  //Funker
void oled_goto_column(int column);  //Funker
void oled_home(void);   //Goes to top left
void oled_pos(int row,int column); //Goes to row,col

/*Draws to SRAM, using dual buffering*/
void oled_sram_write_d(uint8_t adr, uint8_t data); //writes 1 data
void oled_sram_write_char(unsigned char c);
void oled_sram_write_string(char* str);
void oled_sram_reset(void);
void oled_sram_menu(Menu* menu);
void oled_sram_clear_line(int line);
void oled_sram_arrow(uint8_t line);
void oled_draw();

#endif
