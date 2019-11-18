/*!@file
* This file contains functions to create and run the OLED menu.
*/
#define F_CPU 4915200

#include <util/delay.h>
#include <stdlib.h>

#include "oled_driver.h"
#include "can_driver.h"
#include "games.h"
#include "space_runner.h"
#include "menu.h"

static Menu* main_menu;
static Menu* current_menu;

static uint8_t current_line;
static uint8_t selected_song;

Menu* menu_make_sub_menu(Menu* parent_menu, char* name, char* header, char* info, void (*function)(char*)){
  Menu* new_menu = malloc(sizeof(Menu));
  new_menu->name            = name;
  new_menu->header          = header;
  new_menu->info            = info;
  new_menu->parent_menu     = parent_menu;
  new_menu->sub_menu        = NULL;
  new_menu->fun_ptr         = function;
  new_menu->num_sub_menu    = 0;
  //update parent menu

  parent_menu->num_sub_menu += 1;
  uint8_t nsm = parent_menu->num_sub_menu;
  parent_menu->sub_menu = realloc(parent_menu->sub_menu,sizeof(Menu*)*nsm);
  parent_menu->sub_menu[nsm-1] = new_menu;
  return new_menu;
}

void change_difficulty(char* info){
  CANmsg diff;
  diff.id = 3;
  diff.length = 1;
  if (info == "BI" ) {
    current_menu->info = "Dragvoll";
    diff.data[0] = 1;
  }
  else if (info == "Dragvoll") {
    current_menu->info = "NTH";
    diff.data[0] = 2;
  }
  else {
    current_menu->info = "BI";
    diff.data[0] = 0;
  }
  can_send_msg(&diff);
}

void music_run(char* name){
  CANmsg music_msg;
  music_msg.id = 7;
  music_msg.length = 2;

  if (name == "Play Music"){
    music_msg.data[0] = 1;
    current_menu->sub_menu[current_line-1]->name = "Stop Music";
  }
  else {
    music_msg.data[0] = 0;
    current_menu->sub_menu[current_line-1]->name = "Play Music";
  }
  can_send_msg(&music_msg);
}

Menu* menu_init() {

  //main menu creation
  main_menu = malloc(sizeof(Menu));
  main_menu->name         = "Main Menu";
  main_menu->header       = "";
  main_menu->info         = "";
  main_menu->parent_menu  = NULL;
  main_menu->num_sub_menu = 0;
  main_menu->sub_menu     = NULL;
  current_menu            = main_menu;

  /*Main Menu options*/
  Menu* ping_pong     = menu_make_sub_menu(main_menu, "Ping Pong!","","BI",NULL);
  Menu* space_runner  = menu_make_sub_menu(main_menu, "Space Runner", "", "",NULL);
  Menu* settings      = menu_make_sub_menu(main_menu, "Settings", "", "", NULL);

  /*Pingpong submenus*/
  Menu* start_game  = menu_make_sub_menu(ping_pong, "Start","","",&play_pingpong);
  Menu* high_score  = menu_make_sub_menu(ping_pong, "High Score", "Select Level", "",NULL);
  Menu* difficulty  = menu_make_sub_menu(ping_pong, "Difficulty", "", "",&change_difficulty);

  /*Spacerunner submenus*/
  Menu* sr_start    = menu_make_sub_menu(space_runner, "Start SR","","",&sr_play);
  Menu* music        = menu_make_sub_menu(space_runner, "Play Music", "", "", &music_run);

  /*Settings submenus*/
  Menu* flip_col    = menu_make_sub_menu(settings, "Flip Colors", "", "", &oled_flip_colors);
  Menu* set_bright = menu_make_sub_menu(settings, "Set brightness", "", "", &oled_set_brightness);

  //oled initialisering
  current_line = 1; //top line
  selected_song = 99;

  oled_sram_menu(current_menu);
  oled_sram_arrow(current_line);

  //return main_menu;
}

void menu_run_functions(){

  /*Change difficulty*/
  if (current_menu->sub_menu[current_line-1]->fun_ptr == &change_difficulty) {
      /*Update info lable at bottom of Ping Pong menu with the current difficulty*/
      (*current_menu->sub_menu[current_line-1]->fun_ptr)(current_menu->info);
  }

  else if(current_menu->sub_menu[current_line-1]->fun_ptr == &play_pingpong) {
      (*current_menu->sub_menu[current_line-1]->fun_ptr)("Horefant");
  }

  else if(current_menu->sub_menu[current_line-1]->fun_ptr == &sr_play) {
    (*current_menu->sub_menu[current_line-1]->fun_ptr)("kmp");
  }

  else if(current_menu->sub_menu[current_line-1]->fun_ptr == &oled_set_brightness) {
    (*current_menu->sub_menu[current_line-1]->fun_ptr)("");
  }

  else if(current_menu->sub_menu[current_line-1]->fun_ptr == &oled_flip_colors) {
    (*current_menu->sub_menu[current_line-1]->fun_ptr)("");
  }

  else if(current_menu->sub_menu[current_line-1]->fun_ptr == &music_run) {
    (*current_menu->sub_menu[current_line-1]->fun_ptr)(current_menu->sub_menu[current_line-1]->name);
  }
}

void menu_run() {
    switch (joystick_get_direction()) {
    case RIGHT:
      //Om submenyen vi prøver å velge har en submeny
      if (current_menu->sub_menu[current_line-1]->sub_menu != NULL){
        current_menu = current_menu->sub_menu[current_line-1];
        current_line = 1;
        oled_sram_menu(current_menu);
        oled_sram_arrow(current_line);
      }
      else if (current_menu->sub_menu[current_line-1]->fun_ptr != NULL){
        menu_run_functions();
        //current_line = 1;
        oled_sram_menu(current_menu);
        oled_sram_arrow(current_line);
      }
      _delay_ms(150);
      break;

    case LEFT:
      if (current_menu->parent_menu != NULL){
        current_menu = current_menu->parent_menu;
        current_line = 1;
        oled_sram_menu(current_menu);
        oled_sram_arrow(current_line);
        //_delay_ms(200);
      }
      break;

    case UP:
      if (current_line > 1) {
        current_line --; //radnr minker når vi går oppover
        oled_sram_menu(current_menu);
        oled_sram_arrow(current_line);
        //_delay_ms(200);
      }

      break;

    case DOWN:
      if (current_line < current_menu->num_sub_menu) {
        current_line ++;
        oled_sram_menu(current_menu);
        oled_sram_arrow(current_line);
        //_delay_ms(200);
      }

      break;

    case NEUTRAL:
      break;

    default:
      break;
  }
  _delay_ms(150);
}
