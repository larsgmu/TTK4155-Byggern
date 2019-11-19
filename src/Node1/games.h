/*!@file
* This file contains functions to play the pingpong game.
*/

#ifndef GAMES_H
#define GAMES_H

#include <stdint.h>

/*!
*@brief Starts the pingpong game.
*OLED is updated and CAN message is sent to Node2 to start pingpong game.
*Returns to menu when CAN message is received from Node2.
*/
void play_pingpong();

#endif
