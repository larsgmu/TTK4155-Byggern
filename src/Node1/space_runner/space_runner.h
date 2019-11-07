/*!@file
* Space Runner 
*/
#ifndef SPACE_RUNNER
#define SPACE_RUNNER

#include "joystick_driver.h"

/*
The game will have a main loop which will be broken when the runner has 0 lives. 

The draw_map() will continously update the MAP matrix with the runner and obstacles represented as 1's.
I guess this eliminates draw_runner() 



When jumping, jump flag is activated. This is deactivated once hit ground

The map_to_mem() will transform the MAP matrix into SRAM-format and write to SRAM. Gonna do this pixel by pixel
*/

#define RUNNER_WIDTH 	12
#define RUNNER_HEIGHT 	16
#define OBSTACLE_DIM 	8
#define OBSTACLE_NO		3

/*The Space Runner*/
typedef struct SpaceRunner_struct {
	uint8_t 	posy ; 									//Y-position of bottommost pixel
	uint8_t		posx ;  								//X-position of leftmost pixel of sprite
	uint8_t 	velx ; 									//Running speed		(Changes the speed of obstacles)
	float 		vely ; 									//Jump speed vector
	uint8_t		sprite[RUNNER_HEIGHT][RUNNER_WIDTH];	//The player is 16x8 pixels
} Runner;

/*Obstacle*/
typedef struct Obstacle_struct {
	uint8_t 	posx;									//X-position of obstacle left 
	uint8_t		height 	= 1	;							//The obstacles may be higher than 1 unit
	uint8_t 	sprite[OBSTACLE_DIM][OBSTACLE_DIM]	;	//8x8 pixels
} Obstacle;

typedef struct Obstacle_list_struct {
	uint8_t		size 	= 0 ;
	Obstacle 	obstacles[OBSTACLE_NO];
} Obstable_list;


void sr_sprite_test(Runner* runner);

/*
Initializes OLED to be used for this game.
Initializes the game map and Runner
Draws empty map with sprite and ground
*/
void sr_init(Runner* runner);

//Draw map with player and obstacle
void sr_draw_map();

//Renders obstacles
void sr_gen_obst(Runner* runner);

//Player jumps, changes sprite
void sr_jump(Runner* runner);

//Player collides with obstacles
void sr_crash();

/*
!This function runs every time step!
Update runner position
Update obstacle position
Check for collsion
*/
void sr_run(Runner* runner, Joystick* joy);

//Mapping the 64*128-matrix to SRAM
void sr_map_to_mem();

/*Draws OLED from memory*/
void sr_mem_to_oled();

/*Game loop*/
void sr_play(Joystick* joy);




#endif
