/*!@file
* Space Runner
*/
#ifndef SPACE_RUNNER_H
#define SPACE_RUNNER_H

#include "slider_driver.h"
#include "joystick_driver.h"
#include <stdint.h>

#define SR_RUNNER_WIDTH 		6
#define SR_RUNNER_HEIGHT 		16
#define SR_OBSTACLE_DIM 		8
#define SR_OBSTACLE_NO			2

/*!
*@brief Struct containing the player info; X/Y-position, X/Y-velocity and Player sprite.
*/
typedef struct SpaceRunner_struct {
  	uint8_t 	posy ; 									//Y-position of bottommost pixel
  	uint8_t		posx ;  								//X-position of leftmost pixel of sprite
  	uint8_t 	velx ; 									//Running speed		(Changes the speed of obstacles)
  	float 		vely ; 									//Jump speed vector
  	uint8_t		sprite[SR_RUNNER_HEIGHT][SR_RUNNER_WIDTH];	//The player is 16x10 pixels
} sr_Runner;

/*!
*@brief Struct containing X-pos and height (in units of 8 pixels) of a single obstacle.
*/
typedef struct Obstacle_struct {
  	int8_t 		posx;							      //X-position, left of obstacle. Shifted 10 pixels to Right!
  	uint8_t		height;							    //The obstacles may be higher than 1 unit
} sr_Obstacle;

/*!
*@brief Struct containing info about how many obstacles there are in the game frame at current time.
*/
typedef struct Obstacle_list_struct {
  	uint8_t				size;
  	sr_Obstacle 	obstacles[SR_OBSTACLE_NO];
} sr_Obstacle_list;

/*!
*@brief Clears SRAM and draws ground.
*/
void sr_sram_init();

/*!
*@Initialize the runner sprite.
*@param[in] @c sr_Runner* runner -> Pointer to player struct.
*/
void sr_sprite_init(sr_Runner* runner);

/*!
*@brief Initialize game with player and obstacle info.
*@param[in] @c char* diff -> Sets difficulty, changing runner speed.
*@param[in] @c sr_Runner* runner -> Pointer to player struct.
*@param[in] @c sr_Obstacle_list* o_list -> Pointer to list of obstacles.
*/
void sr_init(char* diff, sr_Runner* runner, sr_Obstacle_list* o_list);

/*!
*@brief Game loop that runs until player has crashed and Game Over-flag is set.
*@param[in] @c char* diff -> String containing difficulty; Easy or hard. Changes running speed.
*@param[in] @c Joystick* joy -> Continiously updated Joystick struct pointer
*@param[in] @c Slider* slider -> Slider struct pointer. Not in use.
*/
void sr_play(char* diff, Joystick* joy, Slider* slider);

/*!
*@brief Runs every time step. Updates runner Y-position according to Y-velocity and gravity.
* Runs jump-function if joystick is pushed up.
* Updates obstacles X-position according to runner X-speed. Removes obstacle from list if out of bounds.
* Runs a random function for when to generate obstacle.
*@param[in] @c sr_Runner* runner -> Pointer to player struct containing velocity.
*@param[in] @c Joystick* joy -> Pointer to a continiously updated joystick struct.
*@param[in] @c sr_Obstacle_list* o_list -> Pointer to list of obstacles.
*/
void sr_run(sr_Runner* runner, sr_Obstacle_list* o_list, Joystick* joy, Slider* slider);

/*!
*@brief Draws runner from struct x-position and y-position info. Clears old pixels.
*@param[in] @c sr_Runner* runner -> Pointer to player struct.
*/
void sr_draw_runner(sr_Runner* runner);

/*!
*@brief Draws obstacles. Iterates through Obstacle List containing info about all current obstacles.
*@param[in] @c sr_Runner* runner -> Pointer to player struct containing velocity.
*@param[in] @c sr_Obstacle_list* o_list -> Pointer to list of obstacles.
*/
void sr_draw_obstacle(sr_Runner* runner, sr_Obstacle_list* o_list);

/*!
*@brief Generates a new obstacle and adds it to list.
*@param[in] @c sr_Obstacle_list* o_list -> Pointer to list of obstacles.
*/
void sr_gen_obstacle(sr_Obstacle_list* o_list);

/*!
*@brief Activates when joystick is pushed up. If player is not in air, set Y-velocity to jump speed.
*@param[in] @c sr_Runner* runner -> Pointer to player struct.
*/
void sr_jump(sr_Runner* runner);

/*!
*@brief Activates when player collides with an obstacle. Print score, make screen white. Set Game Over-flag to 1.
*/
void sr_crash();






#endif
