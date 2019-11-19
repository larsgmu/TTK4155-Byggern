/*!@file
* This file contains functions to create and run the OLED menu.
*/
#define F_CPU 4915200

#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>

#include "oled_driver.h"
#include "can_driver.h"
#include "games.h"
#include "joystick_driver.h"
#include "menu.h"
#include "slider_driver.h"
#include "space_runner.h"
#include "eeprom_driver.h"

/*Global variables*/
static Menu* main_menu;
static Menu* current_menu;

static uint8_t current_line;
static uint8_t selected_song;


/*-------------------------------------------------------*/
/*Function declarations*/

/*!
*@brief Creates a new submenu.
*@param[in] @c Menu* parent_menu -> Pointer to parent menu.
*@param[in] @c char* name -> Name of menu
*@param[in] @c char* header -> Header to display at top page. Use "" if same as name.
*@param[in] @c char* info -> Text to display at bottom page. Use "" if not desired.
*@param[in] void (*function)(void)) -> Function to perform if menu is selected. Use NULL if no function.
*@return @c Menu* -> Returns a pointer to the newly constructed menu struct.
*/
Menu* menu_make_sub_menu(Menu* parent_menu, char* name, char* header, char* info, void (*function)(void));

/*!
*@brief Executes function connected to selected menu.
*@param[in] @c Joystick* joy -> Pointer to game controller joystick struct.
*/
void menu_run_functions();

/*!
*@brief Changes game difficulty.
*/
void change_difficulty();

/*!
*@brief Displays current highscore in Ping Pong Game.
*/
void print_pingpong_score();

/*!
*@brief Displays current highscore in Space Runner Game.
*/
void print_sr_score();

/*!
*@brief Either plays or stops music.
*/
void music_run();

/*-------------------------------------------------------*/
/*Function implementations*/

void menu_init() {
  /*Main Menu creation*/
  main_menu = malloc(sizeof(Menu));
  main_menu->name         = "Main Menu";
  main_menu->header       = "";
  main_menu->info         = "";
  main_menu->parent_menu  = NULL;
  main_menu->num_sub_menu = 0;
  main_menu->sub_menu     = NULL;
  current_menu            = main_menu;

  /*Main Menu options*/
  Menu* ping_pong     = menu_make_sub_menu(main_menu, "Ping Pong!","","Dragvoll",NULL);
  Menu* space_runner  = menu_make_sub_menu(main_menu, "Space Runner", "", "",NULL);
  Menu* settings      = menu_make_sub_menu(main_menu, "Settings", "", "", NULL);

  /*Pingpong submenus*/
  Menu* start_game    = menu_make_sub_menu(ping_pong, "Start","","",&play_pingpong);
  Menu* difficulty    = menu_make_sub_menu(ping_pong, "Difficulty", "", "",&change_difficulty);
  Menu* high_score    = menu_make_sub_menu(ping_pong, "High Score", "", "", &print_pingpong_score);
  /*Spacerunner submenus*/
  Menu* sr_start      = menu_make_sub_menu(space_runner, "Start SR","","",&sr_play);
  Menu* music         = menu_make_sub_menu(space_runner, "Play Music", "", "", &music_run);
  Menu* high_score_sr = menu_make_sub_menu(space_runner, "High Score", "", "", &print_sr_score);

  /*Settings submenus*/
  Menu* flip_col      = menu_make_sub_menu(settings, "Flip Colors", "", "", &oled_flip_colors);
  Menu* set_bright    = menu_make_sub_menu(settings, "Set Brightness", "", "", &oled_set_brightness);

  /*Initializing Oled */
  current_line  = 1; //top line
  selected_song = 99;

  oled_sram_menu(current_menu);
  oled_sram_arrow(current_line);
}

Menu* menu_make_sub_menu(Menu* parent_menu, char* name, char* header, char* info, void (*function)(void)){
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
  parent_menu->sub_menu         = realloc(parent_menu->sub_menu,sizeof(Menu*)*nsm);
  parent_menu->sub_menu[nsm-1]  = new_menu;
  return new_menu;
}

void menu_run() {
    switch (joystick_get_direction()) {
    case RIGHT:
      /*If the submenu has submenues and is not a function*/
      if (current_menu->sub_menu[current_line-1]->sub_menu != NULL){
        current_menu = current_menu->sub_menu[current_line-1];
        current_line = 1;
        oled_sram_menu(current_menu);
        oled_sram_arrow(current_line);
      }
      /*If the submenu is a function pointer*/
      else if (current_menu->sub_menu[current_line-1]->fun_ptr != NULL){
        menu_run_functions();
        oled_sram_menu(current_menu);
        oled_sram_arrow(current_line);
      }
      break;

    case LEFT:
      if (current_menu->parent_menu != NULL){
        current_menu = current_menu->parent_menu;
        current_line = 1;
        oled_sram_menu(current_menu);
        oled_sram_arrow(current_line);
        _delay_ms(200);
      }
      break;

    case UP:
      if (current_line > 1) {
        current_line --;
        oled_sram_menu(current_menu);
        oled_sram_arrow(current_line);
      }
      break;

    case DOWN:
      if (current_line < current_menu->num_sub_menu) {
        current_line ++;
        oled_sram_menu(current_menu);
        oled_sram_arrow(current_line);
      }
      break;

    case NEUTRAL:
      break;

    default:
      break;
  }
  _delay_ms(150);
}

void menu_run_functions(){

  /*Change difficulty*/
  if (current_menu->sub_menu[current_line-1]->fun_ptr == &change_difficulty) {
      /*Update info label at bottom of Ping Pong menu with the current difficulty*/
      (*current_menu->sub_menu[current_line-1]->fun_ptr)();
  }
  /*Play ping pong*/
  else if(current_menu->sub_menu[current_line-1]->fun_ptr == &play_pingpong) {
      (*current_menu->sub_menu[current_line-1]->fun_ptr)();
  }
  /*Play Space runner*/
  else if(current_menu->sub_menu[current_line-1]->fun_ptr == &sr_play) {
    (*current_menu->sub_menu[current_line-1]->fun_ptr)();
  }
  /*Change OLED brightness*/
  else if(current_menu->sub_menu[current_line-1]->fun_ptr == &oled_set_brightness) {
    (*current_menu->sub_menu[current_line-1]->fun_ptr)();
  }
  /*Flip OLED colors*/
  else if(current_menu->sub_menu[current_line-1]->fun_ptr == &oled_flip_colors) {
    (*current_menu->sub_menu[current_line-1]->fun_ptr)();
  }
  /*Play music*/
  else if(current_menu->sub_menu[current_line-1]->fun_ptr == &music_run) {
    (*current_menu->sub_menu[current_line-1]->fun_ptr)();
  }
  /*Display Highscore, Ping Pong*/
  else if(current_menu->sub_menu[current_line-1]->fun_ptr == &print_pingpong_score) {
    (*current_menu->sub_menu[current_line-1]->fun_ptr)();
  }
  /*Display Highscore, Space Runner*/
  else if(current_menu->sub_menu[current_line-1]->fun_ptr == &print_sr_score) {
    (*current_menu->sub_menu[current_line-1]->fun_ptr)();
  }
}

void change_difficulty() {
  CANmsg diff;
  diff.id     = 3;
  diff.length = 1;

  char* info = current_menu->info;
  if (info == "NTH" ) {
    current_menu->info = "Dragvoll";
    diff.data[0] = 0;
  }
  else if (info == "Dragvoll") {
    current_menu->info = "NTH";
    diff.data[0] = 1;
  }

  can_send_msg(&diff);
  oled_pos(5,1);
  oled_sram_write_string("Difficulty:");
  oled_pos(7,1);
  oled_sram_write_string(current_menu->info);
  oled_draw();
  CANmsg dummy;
  dummy.id      = 6;
  dummy.length  = 1;
  dummy.data[0] = 0;
  can_send_msg(&dummy);
  _delay_ms(1500);

}

void print_pingpong_score(){
  char score[5];
  itoa(eeprom_read(HIGHSCORE_PINGPONG_ADDR), score, 10);
  oled_pos(5,1);
  oled_sram_write_string("High Score: ");
  oled_sram_write_string(score);
  oled_draw();
  _delay_ms(1000);
}

void print_sr_score(){
  char score[5];
  itoa((uint16_t)(eeprom_read(HIGHSCORE_SRH_ADDR)<<8) + (eeprom_read(HIGHSCORE_SRL_ADDR)), score, 10);
  oled_pos(5,1);
  oled_sram_write_string("High Score: ");
  oled_sram_write_string(score);
  oled_draw();
  _delay_ms(1000);
}

void music_run(){
  CANmsg music_msg;
  music_msg.id      = 7;
  music_msg.length  = 1;

  char* name = current_menu->sub_menu[current_line-1]->name;
  if (name == "Play Music") {
    music_msg.data[0] = 1;
    current_menu->sub_menu[current_line-1]->name = "Stop Music";

  }
  else  {
    music_msg.data[0] = 0;
    current_menu->sub_menu[current_line-1]->name = "Play Music";
  }
  can_send_msg(&music_msg);
}
