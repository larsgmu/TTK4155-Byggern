/*!@file
* This file contains the different games to play.
*/

#ifndef GAMES_H
#define GAMES_H

#include <stdint.h>

/*!
*@brief Starts the pingpong game.
*OLED is updated and CAN message is sent to Node2 to start pingpong game.
*Returns to menu when CAN message is received from Node2.
*@param[in] char* player -> Player name.
*@param[in] Joystick* joy -> Pointer to game controller joystick struct.
*/
void play_pingpong();

#endif
