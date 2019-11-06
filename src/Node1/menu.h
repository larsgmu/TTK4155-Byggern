/*!@file
* OLED menu
*/
#ifndef MENU_H
#define MENU_H
#include <stdint.h>
#include "joystick_driver.h"
typedef struct Menu_struct Menu;
typedef struct Menu_struct {
  char* header;
  char* info;
  char* name;
  Menu* parent_menu;
  Menu* sub_menu[6];
  uint8_t num_sub_menu;
  void (*fun_ptr)(char*);
  void (*fun_ptr2)(char*, Joystick*);
} Menu;

Menu* menu_make_sub_menu(Menu* parent_menu, char* name, char* header, char* info, void (*function)(char*));
Menu* menu_make_sub_menu2(Menu* parent_menu, char* name, char* header, char* info, void (*function)(char*,Joystick*));
Menu* menu_init();
void menu_run(Joystick* joy);
void menu_run_functions(Joystick* joy);
void customLine(int line, char*, int lenght);

#endif
