/*!@file
* This file contains functions to create and run the OLED menu.
*/
#ifndef MENU_H
#define MENU_H

typedef struct Menu_struct Menu; //TRENGS DENNNE?

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
*@brief Creates the main menu and all submenus.
* Updates current line to top line.
*/
void menu_init();

/*!
*@brief Runs the menu.
*Jostick direction is updated and corresponding menu action is executed.
*Writes the new menu to display to SRAM.
*/
void menu_run();

#endif
