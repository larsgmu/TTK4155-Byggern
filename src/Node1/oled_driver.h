/*!@file
* This file contains functions to display data on the game controller OLED screen.
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

/*!
*@brief Struct containing current state of OLED and current cursor location.
* Also contains info wether or not the OLED display data have been changed.
*/
struct oled_data_marker_struct
{
    uint8_t LINE; /*!<Current page */
    uint8_t COL; /*!<Current column */
    uint8_t CHANGED; /*!<If picture is changed*/
    uint8_t COLOR;
    uint8_t BRIGHTNESS;
};

/*!
*@brief Initializes the OLED screen.
* Configure OLED screen to horizontal addressing mode.
*/
void oled_init();

/*!
*@brief Changes oled screen brightness.
*/
void oled_set_brightness();

/*!
*@brief Flips oled colors.
*/
void oled_flip_colors();

/*!
*@brief Updates current state struct and chooses specified page.
*@param[in] @c int line -> Page to move to.
*/
void oled_goto_line(uint8_t line);

/*!
*@brief Updates current state struct and chooses specified column.
*@param[in] @c int column -> Column to move to.
*/
void oled_goto_column(uint8_t column);

/*!
*@brief Updates current state struct and goes to specified page and column.
*@param[in] int row -> Page to go to.
*@param[in] int Column -> Column to go to.
*/
void oled_pos(uint8_t row, uint8_t column); //Goes to row,col

/*!
*@brief Updates current state and writes a char string to OLED-SRAM.
*@param[in] @c char* str -> Char strin to write to OLED-SRAM.
*/
void oled_sram_write_string(char* str);

/*!
*@brief Resets the entire OLED-SRAM
*/
void oled_sram_reset(void);

/*!
*@brief Writes the specified menu to OLED-SRAM.
*@param[in] @c Menu* menu -> Menu to write to OLED-SRAM.
*/
void oled_sram_menu(Menu* menu);

/*!
*@brief Writes the cursor to OLED-SRAM.
*@param[in] @c uint8_t line -> Page to write the cursor on.
*/
void oled_sram_arrow(uint8_t line);

/*!
*@brief Draws the entire OLED screen from the data in OLED-SRAM.
*/
void oled_draw();

#endif
