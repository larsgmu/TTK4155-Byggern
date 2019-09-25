#ifndef MENU_H
#define MENU_H

#include <stdint.h>
typedef struct Menu_struct Menu;
typedef struct Menu_struct {
  char* name;

  Menu* parent_menu;
  Menu* sub_menu[7];
  uint8_t num_sub_menu;
}Menu;

Menu* menu_make_sub_menu(Menu* parent_menu, char* name);
Menu* menu_init();
void menu_run(struct joystick* joy);

#endif
