/*!@file
* This file contains the different games to play.
*/

#ifndef GAMES_H
#define GAMES_H

#include <stdint.h>
#include "joystick_driver.h"

/*!
*@brief Struct containing the name and score of the current player.
*/
typedef struct Game_info_struct{
  char* player_name;
  uint8_t score;
}Game_info;


/*!
*@brief Initializes TIMER3 to keep track of pingpong score.
*/
void pingpong_timer_init();

/*!
*@brief Counts the score of current game.
*/
void pingpong_score();

/*!
*@brief Starts the pingpong game.
*OLED is updated and CAN message is sent to Node2 to start pingpong game.
*Returns to menu when CAN message is received from Node2.
*@param[in] char* player -> Player name.
*@param[in] Joystick* joy -> Pointer to game controller joystick struct.
*/
void play_pingpong(char* player, Joystick* joy);

#endif
