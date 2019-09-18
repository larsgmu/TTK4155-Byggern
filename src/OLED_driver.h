/*
 * OLED_driver.h
 *
 * Module for handling printing to the OLED screen. Contains two parts; one for writing directly
 * to the OLED and the other uses the SRAM to store the next data to be printed. This enables
 * dual frame buffering.
 *
 * Created: 20.09.2017 12:23:16
 *  Author: aleksra, Bendik Standal, Alexander Johansen
 */


#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_
#define ext_ram         ((volatile char*) 0x1800)
#define ext_ram_size    0x800
#define ext_adc         ((volatile char*) 0x1400)
#define ext_oled_cmd    ((volatile char*) 0x1000)
#define ext_oled_data ((volatile char*) 0x1200)

#include <stdint.h>
//#include "MENU_system.h"

#define ext_oled_cmd  ((volatile char*) 0x1000)
#define ext_oled_data ((volatile char*) 0x1200)
#define ext_OLED_size 0x200
#define ASCII_OFFSET ((volatile unsigned int) 32)


/*Functions for initializing OLED, where configurations and addressing modes are set,
and OLED_reset for clearing the screen.*/
void OLED_init(void);
void OLED_reset(void);
void OLED_clear_page(uint8_t page);

/*Functions for moving the cursor in the OLED printing. OLED_home makes the cursor move
to the top left corner.*/
void OLED_home(void);
void OLED_goto_page(uint8_t page);
void OLED_goto_column(uint8_t column);
void OLED_pos(uint8_t row, uint8_t col);

/*Functions for printing single characters and cstrings respectively.*/
void OLED_print(unsigned char c);
void OLED_print_str(const char* data);

/*Convert any integer of size <256 to cstring for using the OLED_print_str function.
Should be used right away and not assigned to a variable for later use, because
this returns address of a local variable*/
char* OLED_int_to_str(const int integer);

/*Custom print functions for menus and arrow. Writes to the SRAM. Use OLED_draw() to
draw the screen.*/
// void OLED_print_menu(Menu* menu);
void OLED_draw_arrow(int pos);

/*draws the entire OLED part of the SRAM to the screen in one go.*/
void OLED_draw(void);

/*Functions for writing data to the SRAM instead of directly to the OLED.
Used for dual-frame buffering of the OLED screen.*/
void OLED_SRAM_print(unsigned char c);
void OLED_SRAM_clear_page(uint8_t page);
void OLED_SRAM_RESET(void);
void OLED_SRAM_custom_print(const unsigned char* c);

#endif /* OLED_DRIVER_H_ */
