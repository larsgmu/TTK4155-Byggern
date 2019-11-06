/*!@file
* OLED menu
*/
#include "joystick_driver.h"
#include "menu.h"
#include "oled_driver.h"
#include <util/delay.h>
#include "can_driver.h"
#include <stdlib.h>
#include <string.h>

Menu* main_menu;
Menu* current_menu;

uint8_t current_line;
char* current_difficulty;
Menu* menu_make_sub_menu(Menu* parent_menu, char* name, char* header, char* info, void (*function)(char*) ){
  Menu* new_menu = malloc(sizeof(Menu));
  new_menu->name = name;
  new_menu->header = header;
  new_menu->info = info;
  new_menu->parent_menu = parent_menu;
  for (int i = 0; i < 7; i++) {
    new_menu->sub_menu[i] = NULL;
  }
  new_menu->fun_ptr = function;
  new_menu->num_sub_menu = 0;
  //update parent menu
  parent_menu->num_sub_menu += 1;
  uint8_t nsm = parent_menu->num_sub_menu;
  parent_menu->sub_menu[nsm-1] = new_menu;
  return new_menu;
}

void change_difficulty(char* diff){
  current_difficulty = diff;
  current_menu->parent_menu->info = diff;
}


Menu* menu_init() {

  //main menu creation
  main_menu = malloc(sizeof(Menu));
  current_difficulty = "EASY";
  main_menu->name = "Main Menu";
  main_menu->header = "";
  main_menu->info = "";
  main_menu->parent_menu = NULL;
  main_menu->num_sub_menu = 0;
  for (int i = 0; i < 7; i++) {
    main_menu->sub_menu[i] = NULL;
  }
  current_menu = main_menu;

  Menu* ping_pong = menu_make_sub_menu(main_menu, "Ping Pong!","",current_difficulty,NULL);
  Menu* start_game = menu_make_sub_menu(ping_pong, "Start","","",NULL);
  Menu* difficulty = menu_make_sub_menu(ping_pong, "Difficulty", "Select Level", "",NULL);
  Menu* high_score = menu_make_sub_menu(ping_pong, "High Score", "Select Level", "",NULL);
  Menu* level_1 = menu_make_sub_menu(difficulty, "EASY","","",&change_difficulty);
  Menu* level_2 = menu_make_sub_menu(difficulty, "NORMAL","","",&change_difficulty);
  Menu* level_3 = menu_make_sub_menu(difficulty, "HARD","","",&change_difficulty);
  //oled initialisering
  current_line = 1; //top line
}


void menu_run_functions(){

  /*Change difficulty*/
  if (current_menu->sub_menu[current_line-1]->fun_ptr == &change_difficulty)
  {
      /*Update info lable at bottom of Ping Pong menu with the current difficulty*/
      (*current_menu->sub_menu[current_line-1]->fun_ptr)(current_menu->sub_menu[current_line-1]->name);
  }
}

void menu_run(Joystick* joy) {

  CANmsg joystick_direction_msg;
  joystick_direction_msg.id = 0;
  joystick_direction_msg.length = 1;

  switch (joy->dir) {
    case RIGHT:
      //Om submenyen vi prøver å velge har en submeny
      if (current_menu->sub_menu[current_line-1]->sub_menu[0] != NULL){
        current_menu = current_menu->sub_menu[current_line-1];
        current_line = 1;
        _delay_ms(200);
      }
      if (current_menu->sub_menu[current_line-1]->fun_ptr != NULL ){
          menu_run_functions();
      }
      else {

      }
      joystick_direction_msg.data[0] = 4;

      break;

    case LEFT:
      if (current_menu->parent_menu != NULL){
        current_menu = current_menu->parent_menu;
        current_line = 1;
        _delay_ms(200);
      }
      joystick_direction_msg.data[0] = 3;
      break;

    case UP:
      if (current_line > 1) {
        current_line --; //radnr minker når vi går oppover
        _delay_ms(200);
      }
      joystick_direction_msg.data[0] = 1;

      break;

    case DOWN:
      if (current_line < current_menu->num_sub_menu) {
        current_line ++;
        _delay_ms(200);
      }
      joystick_direction_msg.data[0] = 2;

      break;

    case NEUTRAL:
      joystick_direction_msg.data[0] = 0;
      break;

    default:
      break;
  }
  oled_sram_menu(current_menu);
  oled_sram_arrow(current_line);
}
