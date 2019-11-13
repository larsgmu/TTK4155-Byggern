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

#include "menu.h"



/*!
*@brief Struct containing current state of OLED.
*/
struct oled_data_marker_struct
{
    uint8_t LINE; /*!<Current page */
    uint8_t COL; /*!<Current column */
    int CHANGED; /*!<If picture is changed*/
};
/*!
*@brief Initializes the OLED screen.
* Configure OLED screen to horizontal addressing mode.
*/
void oled_init();

/*!
*@brief Chooses specified OLED command to execute.
*@param[in] @c uint8_t command -> Command to execute.
*/
void oled_write_c(uint8_t command);

/*!
*@brief Writes data to OLED.
*@param[in] @c uint8_t data -> Data to write to OLED
*/
void oled_write_d(uint8_t data);

/*!
*@brief Updates current state struct and chooses specified page.
*@param[in] @c int line -> Page to move to.
*/
void oled_goto_line(int line);

/*!
*@brief Updates current state struct and chooses specified column.
*@param[in] @c int column -> Column to move to.
*/
void oled_goto_column(int column);

/*!
*@brief Updates current state struct goes to top page and left most column.
*/
void oled_home(void);

/*!
*@brief Updates current state struct and goes to specified page and column.
*@param[in] int row -> Page to go to.
*@param[in] int Column -> Column to go to.
*/
void oled_pos(int row,int column); //Goes to row,col

/*!
*@brief Updates current state and writes specified data to given adress.
*@param[in] @c uint8_t adr -> Adress to write data to.
*@param[in] @c uint8_t data -> Data to write to adress.
*/
void oled_sram_write_d(uint8_t adr, uint8_t data); //writes 1 data

/*!
*@brief Updates current state and writes a character to OLED-SRAM.
*@param[in] @c unsigned char c -> Charachter to write to OLED-SRAM.
*/
void oled_sram_write_char(unsigned char c);

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
*@brief Resets the specified page of the OLED-SRAM.
*@param[in] @c int line -> Page to reset.
*/
void oled_sram_clear_line(int line);

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
