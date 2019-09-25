#include "joystick_driver.h"
#include "menu.h"
#include "oled_driver.h"
#include <util/delay.h>

Menu* main_menu;
Menu* current_menu;

uint8_t current_line;

Menu* menu_make_sub_menu(Menu* super_menu, char* name) {
  //new submenu creation
  // Menu* new_menu = malloc(sizeof(Menu));
  // new_menu->name = name;
  // new_menu->super_menu = super_menu;
  // new_menu->sub_menu = NULL;
  // new_menu->num_sub_menu = 0;

  //update super_menu
  //super_menu->num_sub_menu += 1;
  // uint8_t nsm = super_menu->num_sub_menu;
  // super_menu->sub_menu = realloc(super_menu->sub_menu,sizeof(Menu) * nsm);
  // super_menu->sub_menu[nsm-1] = new_menu;
  // return new_menu;
  Menu* new_menu = malloc(sizeof(Menu));
  new_menu->name = name;
  new_menu->super_menu = super_menu;
  for (int i = 0; i < 7; i++) {
    new_menu->sub_menu[i] = NULL;
  }
  new_menu->num_sub_menu = 0;
  //update parent menu
  super_menu->num_sub_menu += 1;
  uint8_t nsm = super_menu->num_sub_menu;
  super_menu->sub_menu[nsm-1] = new_menu;
  return new_menu;
}

Menu* menu_init() {

  //main menu creation
  main_menu = malloc(sizeof(Menu));
  main_menu->name = "main menu";
  main_menu->super_menu = NULL;
  main_menu->num_sub_menu = 0;
  for (int i = 0; i < 7; i++) {
    main_menu->sub_menu[i] = NULL;
  }
  current_menu = main_menu;

  //Bare for å teste om det funker
  Menu* sub_mem_1 = menu_make_sub_menu(main_menu, "sub_menu_1");
  Menu* sub_mem_2 = menu_make_sub_menu(main_menu, "sub_menu_2");
  Menu* sub_mem_3 = menu_make_sub_menu(main_menu, "sub_menu_3");
  Menu* sub_mem_1_1 = menu_make_sub_menu(sub_mem_1, "sub_menu_1_1");
  Menu* sub_mem_1_2 = menu_make_sub_menu(sub_mem_1, "sub_menu_1_2");

  //oled initialisering
  current_line = 1; //top line
  oled_print_menu(main_menu, current_line);
  oled_print_arrow(current_line);

}

void menu_run(struct joystick* joy) {
  switch (joy->dir) {
    case RIGHT:
      if (current_menu->sub_menu != NULL){
        current_menu = current_menu->sub_menu[current_line-1];
        oled_print_menu(current_menu, current_line);
        oled_print_arrow(1);
        current_line = 1;
      }
      _delay_ms(200);
      break;

    case LEFT:
      if (current_menu->super_menu != NULL){
        current_menu = current_menu->super_menu;
        oled_print_menu(current_menu, current_line);
        oled_print_arrow(1);
        current_line = 1;
      }
      _delay_ms(200);

      break;

    case UP:
      if (current_line > 1) {
        current_line --; //radnr minker når vi går oppover
        oled_print_arrow(current_line);
      }
      _delay_ms(200);
      break;

    case DOWN:
      if (current_line < current_menu->num_sub_menu) {
        current_line ++;
        oled_print_arrow(current_line);
      }
      _delay_ms(200);
      break;

    case NEUTRAL:
      break;

    default:
      break;
  }
}
