#include "space_runner.h"
#include "oled_driver.h"
#include <time.h>

#define MAX_Y 64	//height of OLED	: 8 chars
#define MAX_X 128	//width of OLED		:16 chars

srand(time(0));		//seeds time

static uint8_t 						GRAVITY 			= 4;
static uint8_t						JUMP_SPEED		= 5;
static uint8_t 						GROUND_LEVEL 	= MAX_Y-10;		//ground at 54
volatile static uint8_t 	sr_JUMPFLAG 	= 0;
volatile static uint8_t 	sr_GAMEOVER 	= 0;
volatile static uint16_t 	sr_SCORE 			= 0; 	//max is 65536

/*	64x128 Game map. 1 = stuff, 0 = not stuff.	*/
static uint8_t MAP[MAX_Y][MAX_X];

void sr_init(Runner* runner, Obstacle_list* obst) {
		/*Initialize OLED for game purpose*/
		oled_init();
		//oled_write_c(0x20);       //Set  Memory  Addressing  Mode
		//oled_write_c(0x10);				// 0x10: Page Adressing Mode
	
		sr_GAMEOVER = 0;
		sr_JUMPFLAG = 0;
		sr_SCORE 		= 0;
		
		/*Initialize MAP-matrix with zeroes*/
		for (int y = 0; y < MAX_Y; y++) {
			for (int x = 0; x < MAX_X; x++) {
				MAP[y][x] = 0;
			}
		}
		
    /*Draw ground*/
    for (int y = GROUND_LEVEL; y < MAX_Y; y++) {
        for (int x = 0; x < MAX_X; x++) {
            MAP[y][x] = 1;
        }
    }
		
    /*Init and draw runner*/
    runner->velx 	= 2;
    runner->vely 	= 0;
    runner->posy 	= GROUND_LEVEL-1;
		runner->posx 	= 5;
		sr_sprite_test(runner);		//generates runner matrix
		
		/*Init obstacle list*/
		obst->size = 0; //can delete this
}
	
void sr_sprite_test(Runner* runner) {			//makes a square runner
	for (int y = 0; y < RUNNER_HEIGHT; y++) {
		for (int x = 0; x < RUNNER_WIDTH; x++) {
			runner->sprite[y][x] = 1;
		}
	} 
}

void sr_gen_obst(Runner* runner, Obstacle_list* obst) {
	/*Randomly generates obstacles, maximum 3 at a time.*/
	if (rand()%70 == 2 && obst->size < 2) {
		Obstacle o;
		o->posx 	= MAX_X-OBSTACLE_DIM;
		obst->size ++ ;
		obst[size] = o;
	}
}

void sr_jump(Runner* runner) {
    if sr_JUMPFLAG == 1 { return;	}		//no double jumps
		sr_JUMPFLAG  = 1;
    runner->vely = JUMP_SPEED;
}


void sr_crash() {
	/*Print message to OLED */
	printf("score: %d", sr_SCORE);
	
	/*Draws OLED white*/
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			MAP[y][x] = 1;
			sr_map_to_mem();
			delay_ms(1);
		}
	}
	delay_ms(1000);
	sr_GAMEOVER = 1;
}

void sr_draw_map(Runner* runner, Obstacle_list* obst) {
    //Needs to update obstacles and stuff.
		//Ground is already drawn by init func
		
		/*Clear old map*/
		for (int y = 0; y < GROUND_LEVEL; y++) {
			for (int x = 0; x < MAX_X; x++) {
				MAP[y][x] = 0;
			}
		}

		/*Draw runner if player jumped*/
		//if (sr_JUMPFLAG) {		//Might be buggy because of order of run-function
			uint8_t m = 0;
			uint8_t n = 0;
			for (int y = runner->posy - RUNNER_HEIGHT; y < GROUND_LEVEL; y++) {
				for (int x = runner->posx; x < runner->posx + RUNNER_WIDTH; x++) {
					MAP[y][x] = runner->sprite[m][n];
					n++ ;
				}
				m++ ;
			}
		//}	
		
		/*Draw obstacles*/
		if (obst->size > 0) {
			for (int i = 0; i < obst->size; i++) {
				for (int y = GROUND_LEVEL-OBSTACLE_DIM; y < GROUND_LEVEL; y++) {
					for (int x = obst->obstacles[i]->xpos; x < obst->obstacles[i]->xpos + OBSTACLE_DIM; x++) {
						MAP[y][x] = 1 ;	//draws square obstacle
					}
				}
			}
		}
		
		/*Map to SRAM*/
    sr_map_to_mem();
}


void sr_run(Runner* runner, Joystick* joy, Obstacle_list* obst) {
	/*Update score*/
	sr_SCORE ++;
	
	/* Update Y-velocity */
	if (runner->vely != 0) { //can also check if sr_JUMPFLAG
		runner->vely -= GRAVITY*0.5;
		//If player hits ground
		if (runner->posy == GROUND_LEVEL-1 && sr_JUMPFLAG) {
			runner->vely = 0;
			sr_JUMPFLAG = 0;
		}
	}
	
	/*Jump*/
	if (joy->dir == UP) {
		sr_jump();
	}
	
	/*Update Y-position*/
	if (sr_JUMPFLAG) {
		runner->posy = (int)(runner->posy + vely);		//Values probably not right
	}
	
	/*Update Ostacles' X-position*/
	for (int i = 0; i < obst->size; i ++) {
		obst->obstacles[i]->posx -= velx;
		
		/*Check if crash*/
		if ((runner->posx + RUNNER_WIDTH <= obst->obstacles[i]->posx) && (runner->posy >= GROUND_LEVEL+OBSTACLE_DIM)) {
			sr_crash();
		}
	}
	
	/*Check if Obstacle 1 out of bounds*/
	if (obst->obstacles[0]->posx <= 0) {
		for (int i = 0; i < obst->size-1; i++) {
			obst->obstacle[i] = obst->obstacle[i+1];	//Pushes the obstacles further down the queue
		}
		obst->size --;	//there might be more than (size) obstacles in list but they be ignored
	}
	
	/*Update map*/
	sr_draw_map(runner, obst);
}

static volatile char* oled_sram_adress = (char*)0x1C00;


void sr_map_to_mem() {
	uint8_t c = 0;
	uint8_t p = 0;	
	/*Converts MAP to char arrays for horizontal adressing mode*/
	for (int y = 0; y < MAX_Y; y++) {
		for (int x = 0; x < MAX_X; x++) {
			if ((x-1) % 7) == 0 {
				c++;		//We are now in correct col
			}
		}
		
		
		if ((y-1) % 7) == 0 {
				r++;		//We are now in correct page 
			}
	}
	
	
	
	//oled_sram_adress["NEED CODE HERE"] = MAP[y][x];
	//oled_sram_adress[oled_state.LINE*128 + oled_state.COL + i] = pgm_read_byte(&font8[output][i]);
	
	
}


void sr_oled_draw_pixel(uint8_t adr) {
	
}