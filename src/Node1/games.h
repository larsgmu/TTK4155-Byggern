#ifndef GAMES_H
#define GAMES_H

#include <stdint.h>
#include "joystick_driver.h"
typedef struct Game_info_struct{
  char* player_name;
  uint8_t score;
}Game_info;

void pingpong_timer_init();
void pingpong_score();
void play_pingpong(char* player, Joystick* joy);

#endif
