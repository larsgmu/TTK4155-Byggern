/*!@file
* Space Runner 
*/
#ifndef SPACE_RUNNER
#define SPACE_RUNNER


/*
The game will have a main loop which will be broken when the runner has 0 lives. 
The draw_map() will continously update the MAP matrix with the runner and obstacles represented as 1's.
The map_to_mem() will transform the MAP matrix into SRAM-format and write to SRAM. 


*/

#define MAX_Y 64	//height of OLED
#define MAX_X 128	//width of OLED

int MAP[MAX_Y][MAX_X];	//our game map. 1 = stuff, 0 = not stuff
static int GRAVITY = 4;
static int GROUND_LEVEL = 10;


typedef struct SpaceRunner_struct {
	uint8_t posy; //y-position (0 if not jumping)
	uint8_t velx; //Running speed
	float vely; //Jump speed
} Runner;



//Init game
void sr_init(Runner* runner);

//Draw map
void sr_draw_map();

//Mapping the 64*128-matrix to SRAM
void sr_map_to_mem(int** map);

//Renders obstacles
void sr_draw_obst(Runner* runner);

//Draw / update player
void sr_draw_runner(Runner* runner);

//Player jumps
void sr_jump(Runner* runner);

//Player collides with obstacles
void sr_crash(Runner* runner);












#endif
