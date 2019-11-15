/*!@file
* This file contains functions to create and run the OLED menu.
*/
#define F_CPU 4915200

#include <util/delay.h>
#include <stdlib.h>

#include "menu.h"
#include "oled_driver.h"
#include "can_driver.h"
#include "games.h"
#include "space_runner.h"

Menu* main_menu;
Menu* current_menu;

uint8_t current_line;
char* current_difficulty;

// volatile static uint8_t selected_song = 99;

Menu* menu_make_sub_menu(Menu* parent_menu, char* name, char* header, char* info, void (*function)(char*) ){
  Menu* new_menu = malloc(sizeof(Menu));
  new_menu->name            = name;
  new_menu->header          = header;
  new_menu->info            = info;
  new_menu->parent_menu     = parent_menu;
  new_menu->sub_menu = NULL;
  // for (int i = 0; i < 7; i++) {
  //   new_menu->sub_menu[i]   = NULL;
  // }
  new_menu->fun_ptr         = function;
  new_menu->fun_ptr2        = NULL;
  new_menu->num_sub_menu    = 0;
  //update parent menu

  parent_menu->num_sub_menu += 1;
  uint8_t nsm = parent_menu->num_sub_menu;
  parent_menu->sub_menu = realloc(parent_menu->sub_menu,sizeof(Menu*)*nsm)
  parent_menu->sub_menu[nsm-1] = new_menu;
  return new_menu;
}

Menu* menu_make_sub_menu2(Menu* parent_menu, char* name, char* header, char* info, void (*function)(char*, Joystick*, Slider*)){
  Menu* new_menu = malloc(sizeof(Menu));
  new_menu->name          = name;
  new_menu->header        = header;
  new_menu->info          = info;
  new_menu->parent_menu   = parent_menu;
  // for (int i = 0; i < 7; i++) {
  //   new_menu->sub_menu[i] = NULL;
  // }
  new_menu->sub_menu = NULL;

  new_menu->fun_ptr       = NULL;
  new_menu->fun_ptr2      = function;
  new_menu->num_sub_menu  = 0;

  //update parent menu
  parent_menu->num_sub_menu += 1;
  uint8_t nsm = parent_menu->num_sub_menu;
  parent_menu->sub_menu = realloc(parent_menu->sub_menu,sizeof(Menu*)*nsm)
  parent_menu->sub_menu[nsm-1] = new_menu;
  return new_menu;
}

void change_difficulty(char* diff){
  current_difficulty = diff;
  current_menu->parent_menu->info = diff;
  current_menu->info = diff;
}

// void music_play(char* c){
//   CANmsg start_music;
//   start_music.id = 7;
//   start_music.length = 2;
//   start_music.data[0] = 1;
//   start_music.data[1] = selected_song;
//   can_send_msg(&start_music);
// }
//
// void music_stop(char* c){
//   CANmsg start_music;
//   start_music.id = 7;
//   start_music.length = 2;
//   start_music.data[0] = 0;
//   start_music.data[1] = 99;
//   can_send_msg(&start_music);
// }
//
// void select_song(char* c){
//   selected_song = current_line;
// }

Menu* menu_init() {

  //main menu creation
  main_menu = malloc(sizeof(Menu));
  main_menu->name         = "Main Menu";
  main_menu->header       = "";
  main_menu->info         = "";
  main_menu->parent_menu  = NULL;
  main_menu->num_sub_menu = 0;
  for (int i = 0; i < 7; i++) {
    main_menu->sub_menu[i] = NULL;
  }
  current_menu            = main_menu;
  current_difficulty      = "EASY";

  /*Main Menu options*/
  Menu* ping_pong     = menu_make_sub_menu(main_menu, "Ping Pong!","",current_difficulty,NULL);
  Menu* space_runner  = menu_make_sub_menu(main_menu, "Space Runner", "", "",NULL);
  // Menu* settings      = menu_make_sub_menu(main_menu, "Settings", "", "", NULL);

  /*Pingpong submenus*/
  Menu* start_game  = menu_make_sub_menu2(ping_pong, "Start","","",&play_pingpong);
  Menu* high_score  = menu_make_sub_menu(ping_pong, "High Score", "Select Level", "",NULL);
  Menu* difficulty  = menu_make_sub_menu(ping_pong, "Difficulty", "Select Level", "",NULL);

  /*Difficulty submenus*/
  Menu* level_1     = menu_make_sub_menu(difficulty, "EASY","","",&change_difficulty);
  //Menu* level_2     = menu_make_sub_menu(difficulty, "NORMAL","","",&change_difficulty);
  //Menu* level_3     = menu_make_sub_menu(difficulty, "HARD","","",&change_difficulty);

  /*Spacerunner submenus*/
  Menu* sr_start    = menu_make_sub_menu2(space_runner, "Start SR","","",&sr_play);
  //Menu* sr_diff     = menu_make_sub_menu(space_runner, "Difficulty", "Select speed", "",NULL);

  // /*Settings submenus*/
  // Menu* music       = menu_make_sub_menu(settings, "Media Player", "", "", NULL);
  // Menu* brightness  = menu_make_sub_menu(settings, "Brightness", "", "", &oled_set_brightness);
  // Menu* flip_col    = menu_make_sub_menu(settings, "Flip Colors", "", "", &oled_flip_colors);
  //
  // /*Media Player submenus*/
  // Menu* song_select = menu_make_sub_menu(music, "Select song", "", "", NULL);
  // Menu* play = menu_make_sub_menu(music, "Play!", "", "", &music_play);
  // Menu* stop = menu_make_sub_menu(music, "Stop!", "", "", &music_stop);
  //
  // /*Select song submenus*/
  // Menu* mario = menu_make_sub_menu(song_select, "MARIO", "", "", &select_song);
  // Menu* horer = menu_make_sub_menu(song_select, "HORER", "", "", &select_song);
  // Menu* er = menu_make_sub_menu(song_select, "ER", "", "", &select_song);
  // Menu* greit = menu_make_sub_menu(song_select, "GREIT", "", "", &select_song);
  // Menu* bby = menu_make_sub_menu(song_select, "BBY", "", "", &select_song);

  //oled initialisering
  current_line = 1; //top line

  oled_sram_menu(current_menu);
  oled_sram_arrow(current_line);
}

void menu_run_functions(Joystick* joy, Slider* slider){

  /*Change difficulty*/
  if (current_menu->sub_menu[current_line-1]->fun_ptr == &change_difficulty) {
      /*Update info lable at bottom of Ping Pong menu with the current difficulty*/
      (*current_menu->sub_menu[current_line-1]->fun_ptr)(current_menu->sub_menu[current_line-1]->name);
  }

  else if(current_menu->sub_menu[current_line-1]->fun_ptr2 == &play_pingpong) {
      (*current_menu->sub_menu[current_line-1]->fun_ptr2)("Horefant",joy, slider);
  }

  else if(current_menu->sub_menu[current_line-1]->fun_ptr2 == &sr_play) {
    (*current_menu->sub_menu[current_line-1]->fun_ptr2)("kmp",joy, slider);
  }
  //
  // else if(current_menu->sub_menu[current_line-1]->fun_ptr == &oled_set_brightness) {
  //   (*current_menu->sub_menu[current_line-1]->fun_ptr)("");
  // }
  //
  // else if(current_menu->sub_menu[current_line-1]->fun_ptr == &oled_flip_colors) {
  //   (*current_menu->sub_menu[current_line-1]->fun_ptr)("");
  // }
  //
  // else if(current_menu->sub_menu[current_line-1]->fun_ptr == &music_play) {
  //   (*current_menu->sub_menu[current_line-1]->fun_ptr)("");
  // }
  //
  // else if(current_menu->sub_menu[current_line-1]->fun_ptr == &music_stop) {
  //   (*current_menu->sub_menu[current_line-1]->fun_ptr)("");
  // }
  //
  // else if(current_menu->sub_menu[current_line-1]->fun_ptr == &select_song) {
  //   (*current_menu->sub_menu[current_line-1]->fun_ptr)("");
  // }

}

void menu_run(Joystick* joy, Slider* slider) {
  joystick_run(joy);
  switch (joy->dir) {
    case RIGHT:
      //Om submenyen vi prøver å velge har en submeny
      if (current_menu->sub_menu[current_line-1]->sub_menu[0] != NULL){
        current_menu = current_menu->sub_menu[current_line-1];
        current_line = 1;
        oled_sram_menu(current_menu);
        oled_sram_arrow(current_line);
      }
      else if (current_menu->sub_menu[current_line-1]->fun_ptr != NULL ||
        current_menu->sub_menu[current_line-1]->fun_ptr2 != NULL){
        menu_run_functions(joy, slider);
        current_line = 1;
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
