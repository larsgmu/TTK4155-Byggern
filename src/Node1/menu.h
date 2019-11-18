/*!@file
* This file contains functions to create and run the OLED menu.
*/
#ifndef MENU_H
#define MENU_H

#include <stdint.h>

#include "joystick_driver.h"
#include "slider_driver.h"

typedef struct Menu_struct Menu;

/*!
*@brief Struct containing information about a menu object.
*/
typedef struct Menu_struct {
  char*     header;       /*!<Text to display at top page if not empty.*/
  char*     name;         /*!<Text to display as sub menu when parent menu is selected. Text to display at top page if header is empty*/
  char*     info;   /*!<Text to display at bottom page if not empty*/
  Menu*     parent_menu; /*!<Pointer to parent menu */
  Menu**    sub_menu; /*!<Array containing pointers to sub menus*/
  uint8_t   num_sub_menu;/*!<Variable to count number of submenus conneted to parent menu*/
  void      (*fun_ptr)(void); /*!<Function pointer*/
} Menu;

/*!
*@brief Creates a new submenu.
*@param[in] @c Menu* parent_menu -> Pointer to parent menu.
*@param[in] @c char* name -> Name of menu
*@param[in] @c char* header -> Header to display at top page. Use "" if same as name.
*@param[in] @c char* info -> Text to display at bottom page. Use "" if not desired.
*@param[in] void (*function)(void)) -> Function to perform if menu is selected. Use NULL if no function.
*/
Menu* menu_make_sub_menu(Menu* parent_menu, char* name, char* header, char* info, void (*function)(void));

/*!
*@brief Creates the main menu and all submenus.
* Updates current line to top line.
*/
Menu* menu_init();

/*!
*@brief Runs the menu.
*Jostick direction is updated and corresponding menu action is executed.
*Writes the new menu to display to SRAM.
*/
void menu_run();

/*!
*@brief Executes function connected to selected menu.
*@param[in] @c Joystick* joy -> Pointer to game controller joystick struct.
*/
void menu_run_functions();

#endif
