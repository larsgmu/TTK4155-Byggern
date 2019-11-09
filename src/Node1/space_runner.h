/*!@file
* Space Runner
*/
#ifndef SPACE_RUNNER_H
#define SPACE_RUNNER_H

#include "joystick_driver.h"
#include <stdint.h>

#define SR_RUNNER_WIDTH 		10
#define SR_RUNNER_HEIGHT 		16
#define SR_OBSTACLE_DIM 		8
#define SR_OBSTACLE_NO			3

/*A single bit struct to save space*/
typedef struct Bit_struct {
    unsigned b:1;
} sr_Bit;

/*The Space Runner*/
typedef struct SpaceRunner_struct {
  	uint8_t 	posy ; 									//Y-position of bottommost pixel
  	uint8_t		posx ;  								//X-position of leftmost pixel of sprite
  	uint8_t 	velx ; 									//Running speed		(Changes the speed of obstacles)
  	float 		vely ; 									//Jump speed vector
  	sr_Bit		sprite[SR_RUNNER_HEIGHT][SR_RUNNER_WIDTH];	//The player is 16x12 pixels
} sr_Runner;

/*Obstacle*/
typedef struct Obstacle_struct {
  	int8_t 	  posx;									//X-position of obstacle left
  	uint8_t		height;							//The obstacles may be higher than 1 unit
} sr_Obstacle;

typedef struct Obstacle_list_struct {
  	uint8_t				size;
  	sr_Obstacle 	obstacles[SR_OBSTACLE_NO];
} sr_Obstacle_list;

/*Draws empty map and ground to OLED*/
void sr_sram_init();

void sr_sprite_test(sr_Runner* runner);

/*
Initializes OLED to be used for this game.
Initializes the game map and Runner
Draws empty map with ground
*/
void sr_init(sr_Runner* runner, sr_Obstacle_list* o_list);


void sr_draw_runner(sr_Runner* runner);

void sr_draw_obstacle(sr_Runner* runner, sr_Obstacle_list* o_list);

//Renders obstacles
void sr_gen_obst(sr_Obstacle_list* o_list);

//Player jumps, changes sprite
void sr_jump(sr_Runner* runner);

//Player collides with obstacles
void sr_crash();

/*
!This function runs every time step!
Update runner position
Update obstacle position
Check for collsion#include <stdint.h>
*/
void sr_run(sr_Runner* runner, Joystick* joy, sr_Obstacle_list* o_list);

//Draw map with player and obstacle
//void sr_draw_map();
//Mapping the 64*128-matrix to SRAM
//void sr_map_to_mem();
/*Draws OLED from memory*/
//void sr_mem_to_oled();

/*Game loop*/
void sr_play(Joystick* joy);




#endif
