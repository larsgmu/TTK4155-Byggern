#include 	"space_runner.h"
#include 	"oled_driver.h"
//#include 	<time.h>
#include 	<util/delay.h>
#include	<stdio.h>
#include  <avr/io.h>
#include	<stdlib.h>
#include 	<stdint.h>

#define SR_MAX_Y 64	//height of OLED	: 8 chars
#define SR_MAX_X 128	//width of OLED		:16 chars

static volatile char* oled_command_address 	= (char*)0x1000;
static volatile char* oled_data_address 		= (char*)0x1200;
static volatile char* oled_sram_adress 			= (char*)0x1C00;

static uint8_t 						GRAVITY 			= 4;
static uint8_t						JUMP_SPEED		= 5;
static uint8_t 						GROUND_LEVEL 	= SR_MAX_Y-8;		//ground at 56
volatile static uint8_t 	sr_JUMPFLAG 	= 0;
volatile static uint8_t 	sr_GAMEOVER 	= 0;
volatile static uint16_t 	sr_SCORE 			= 0; 	//max is 65536

static struct oled_data_marker_struct oled_state;

/*	64x128 Game map. 1 = stuff, 0 = not stuff.	*/
//static sr_Bit MAP[SR_MAX_Y][SR_MAX_X];

void sr_init(sr_Runner* runner, sr_Obstacle_list* o_list) {
		/*Initialize OLED for game purposes*/
		oled_init();

		sr_GAMEOVER = 0;
		sr_JUMPFLAG = 0;
		sr_SCORE 		= 0;

		/*Draws map and ground*/
		sr_sram_init();

    /*Init runner*/
    runner->velx 	= 2;
    runner->vely 	= 0;
    runner->posy 	= GROUND_LEVEL-1;
		runner->posx 	= 5;
		sr_sprite_test(runner);		//generates runner matrix

		/*Init obstacle list*/
		o_list->size = 0;
}

//THE SHIT BELOW WORKS
/*   char temp[1] = {0b00000000};
		volatile char col_data[OLED_PAGE_HEIGHT];
    for (int y = OLED_PAGE_HEIGHT-1; y >= 0; y--) {			//reverse iteration . start bottom left
      col_data[OLED_PAGE_HEIGHT - y] = 1;			//col_data[8] = {0,1,1,0,0,1,0,1}
      temp[0] |= (col_data[7-y] << (y));
    }

    int length = 2;
    int COL = 0;
    for(int p = 0; p < 8; p++) {
    for(int j = 0; j < length; j++){
      for(int i = 0; i<OLED_PAGE_HEIGHT; i++){

    //    oled_sram_adress[p*OLED_COLS + COL + i] = temp[0];
      }
      COL += OLED_PAGE_HEIGHT;
    }
    }

  	oled_draw();
    oled_pos(3,101);
    oled_write_d(0b01001001);*/

void sr_sram_init() {
		/*Clear SRAM*/
		for (int p = 0; p < OLED_PAGES; p++) {
			for (int x = 0; x < OLED_COLS; x++) {
				oled_sram_adress[p*OLED_COLS + x] = 0b00000000;
			}
		}
		/*Draw ground*/
		char ground = 0b01001011;
		for (int x = 0; x < OLED_COLS; x++) {
			oled_sram_adress[7*OLED_COLS + x] = ground;
		}
		oled_draw();
}

void sr_draw_runner(sr_Runner* runner) {
		uint8_t bottom_page = OLED_PAGES - 2 ;
		uint8_t top_page 		= OLED_PAGES - 3 ;
		char 		temp[1] 		= {0b00000000};	//Contains info about 8 pixels; 1 col in 1 page
		uint8_t i 					= 0;


		/*If runner hasnt jumped, (is on ground)*/
		if (sr_JUMPFLAG == 0) {
			for (int p = bottom_page; p >= top_page; p--) { //start at p6, then p5
				for (int x = (runner->posx); x < SR_RUNNER_WIDTH+(runner->posx); x++) {	//x<12
					for (int y = 0; y < OLED_PAGE_HEIGHT; y++) {	//y<8
						if ((runner->sprite[y + i*OLED_PAGE_HEIGHT ][x-(runner->posx)].b == 1)) {
							temp[0] |=  (1 << (7-y));
						}
						else {
							temp[0] &= ~(1 << (7-y));
						}
					}
					oled_sram_adress[p*OLED_COLS + x] = temp[0];
					/*Clear page above Runner*/
					oled_sram_adress[4*OLED_COLS + x] = 0x00;
				}
				i++;
			}
		}

		/*If runner has jumped*/
		else if (sr_JUMPFLAG == 1){
			bottom_page 	=  runner->posy / OLED_PAGE_HEIGHT ; //floors value;
			top_page 			= (runner->posy - SR_RUNNER_HEIGHT +1 ) / OLED_PAGE_HEIGHT ;
			temp[0] 			= 0b00000000;
			i 						= 0;
			//Difference between bottom of page and runner y-pos
			uint8_t diff 	= (bottom_page+1)*OLED_PAGE_HEIGHT - runner->posy - 1;

			/*Bottom range: 1<p<6 . Top range: 0<p<5 */
			for (int p = bottom_page; p >= top_page; p--) {
				for (int x = (runner->posx); x < SR_RUNNER_WIDTH + (runner->posx); x++) {
					for (int y = 0; y < OLED_PAGE_HEIGHT; y++) {
						/*If runner is perfectly within 2 pages*/
						if ((runner->posy + 1) % 8 == 0) {
							if ((runner->sprite[y + i*OLED_PAGE_HEIGHT ][x-(runner->posx)].b == 1)) {
								temp[0] |=  (1 << (7-y));	//Light up pixel
							}
							else {
								temp[0] &= ~(1 << (7-y)); //Not light up pixel
							}
							/*Clear out page below and above runner*/
							if (p < 5 && p > 1) {
								oled_sram_adress[(p-1)*OLED_COLS + x] = 0x00;	//above
								oled_sram_adress[(bottom_page+1)*OLED_COLS + x] = 0x00;	//below

							}
						}

						else  { /*If runner is between 3 pages*/
							//printf("Runner is between 3 pages\n\r");
							if (p == bottom_page) {
								if (runner->sprite[y][x-(runner->posx)].b == 1) {
									temp[0] |=  (1 << (7-y-diff));
								}
								else {
									temp[0] &= ~(1 << (7-y-diff));
								}
								if (y < diff) {
									temp[0] &= ~(1 << (7-y));
								}
								//temp[0] &= ~(1 << (7-diff+1));		//clearing old line below runner
								//if (p < 5) { /*Clear page above runner*/
								//	oled_sram_adress[(p-1)*OLED_COLS + x] = 0x00;
								//}
							}
							else if (p == top_page) {
								uint8_t ind = y + 2*OLED_PAGE_HEIGHT-diff;
								if (ind < SR_RUNNER_HEIGHT) {		/*If runner sprite is actually in MAP y-pos*/
									if (runner->sprite[ind][x-(runner->posx)].b == 1) {
										temp[0] |=  (1 << (7-y));
									}
									else {
										temp[0] &= ~(1 << (7-y));
									}
								}
								else { 		/*Set 0 to the rest of page-column*/
									temp[0] &= ~(1 << (7-y));
								}
								if (p >1) { /*Clear page above runner*/
									oled_sram_adress[(p-1)*OLED_COLS + x] = 0x00;
								}
							}
							else { /* If page is between top and bottom */
								if (runner->sprite[7-diff+y][x-(runner->posx)].b == 1) {
									temp[0] |=  (1 << (7-y));
								}
								else {
									temp[0] &= ~(1 << (7-y));
								}
							}
						}
					}
					oled_sram_adress[p*OLED_COLS + x] = temp[0];
					temp[0] 			= 0b00000000;
				}
				i++;
			}
		}
		_delay_ms(1);
		oled_draw();
}

void sr_draw_obstacle(sr_Runner* runner, sr_Obstacle_list* o_list) {
	uint8_t page = 6;
	for (int o = 0; o < o_list->size; o++) {	//Iterate for every o=obstacle in list
		for (int h = 0; h < o_list->obstacles[o].height ; h++) { //For every block in obstacle (1 or 2)
			//For every column in obstacle
			for (int x = o_list->obstacles[o].posx; x < o_list->obstacles[o].posx + SR_OBSTACLE_DIM; x++) {
				oled_sram_adress[(page-h)*OLED_COLS + x] = 0xFF; //Square 8x8 Obstacle
			}
		}
	}
}


void sr_sprite_test(sr_Runner* runner) {			//makes a square runner
	for (int y = 0; y < SR_RUNNER_HEIGHT; y++) {
		for (int x = 0; x < SR_RUNNER_WIDTH; x++) {
				runner->sprite[y][x].b = 1;
		}
	}
}

void sr_gen_obst(sr_Obstacle_list* o_list) {
	sr_Obstacle o;
	o.height = 1; //May be 2
	o.posx 	= SR_MAX_X-SR_OBSTACLE_DIM;
	o_list->size ++ ;
	o_list->obstacles[o_list->size] = o;
}

void sr_jump(sr_Runner* runner) {
    if (sr_JUMPFLAG == 1) { return;	}		//no double jumps
		sr_JUMPFLAG  = 1;
    runner->vely = JUMP_SPEED;
		/*Add another sprite*/
}


void sr_crash() {
	/*Print message to OLED */
	//printf("score: %d", sr_SCORE);

	/*Draws OLED white*/
	for (int y = 0; y < SR_MAX_Y; y++) {
		for (int x = 0; x < SR_MAX_X; x++) {
			//MAP[y][x].b = 1;
			sr_map_to_mem();
			_delay_ms(1);
		}
	}
	_delay_ms(1000);
	sr_GAMEOVER = 1;
}

void sr_run(sr_Runner* runner, Joystick* joy, sr_Obstacle_list* o_list) {
		/*generates a random function for obstacle generating*/
		uint16_t seed = rand();
		srand(seed);

		/*Update score*/
		sr_SCORE ++;

		/* Update Y-velocity */
		if (runner->vely != 0) {
			runner->vely -= GRAVITY*0.1;
			//If player hits ground
			if (runner->posy >= GROUND_LEVEL-1 && sr_JUMPFLAG) {
				runner->vely = 0;
				runner->posy = GROUND_LEVEL-1;
				sr_JUMPFLAG = 0;
				/*Change back to origginal sprite*/
			}
		}

		/*Jump*/
		if (joy->dir == UP) {
			sr_jump(runner);
		}

		/*Update Y-position*/
		if (sr_JUMPFLAG) {
			runner->posy = (int)(runner->posy - runner->vely);		//Values probably not right
		}

		/*Update Ostacles' X-position*/
		for (int i = 0; i < o_list->size; i ++) {
			o_list->obstacles[i].posx -= runner->velx;

			/*Check if crash*/
			if ((runner->posx + SR_RUNNER_WIDTH <= o_list->obstacles[i].posx) && (runner->posy >= GROUND_LEVEL+SR_OBSTACLE_DIM)) {
				sr_crash();
			}
		}

		/*Check if Obstacle 1 out of bounds*/
		if (o_list->obstacles[0].posx <= 1) {
			for (int i = 0; i < o_list->size-1; i++) {
				o_list->obstacles[i] = o_list->obstacles[i+1];	//Pushes the obstacles further down the queue
			}
			o_list->size --;	//there might be more than (SR_DIR = space_runner;size) obstacles in list but they be ignored
		}

		/*Randomly generates obstacles, maximum 3 at a time.*/
		if  ((rand() % 100) == 55 && o_list->size < SR_OBSTACLE_NO) {
			sr_gen_obst(o_list);
		}

		_delay_ms(10);
		/*Update map*/
		sr_draw_runner(runner);
		sr_draw_obstacle(runner, o_list);
}

// void sr_draw_map(sr_Runner* runner, sr_Obstacle_list* o_list) {
//     //Needs to update obstacles and stuff.
// 		//Ground is already drawn by init func
//
// 		/*Clear old map*/
// 		for (int y = 0; y < GROUND_LEVEL; y++) {
// 			for (int x = 0; x < SR_MAX_X; x++) {
// 				//MAP[y][x].b = 0;
// 			}
// 		}
//
// 		/*Draw runner if player jumped*/
// 		//if (sr_JUMPFLAG) {		//Might be buggy because of order of run-function
// 			uint8_t m = 0;
// 			uint8_t n = 0;
// 			for (int y = runner->posy - SR_RUNNER_HEIGHT; y < GROUND_LEVEL; y++) {
// 				for (int x = runner->posx; x < runner->posx + SR_RUNNER_WIDTH; x++) {
// 					//MAP[y][x].b = runner->sprite[m][n].b;
// 					n++ ;
// 				m++ ;
// 			}
// 		}
//
// 		/*Draw obstacles*/
// 		if (o_list->size > 0) {
// 			for (int i = 0; i < o_list->size; i++) {
// 				for (int y = GROUND_LEVEL-SR_OBSTACLE_DIM; y < GROUND_LEVEL; y++) {
// 					for (int x = o_list->obstacles[i].posx; x < o_list->obstacles[i].posx + SR_OBSTACLE_DIM; x++) {
// 						//MAP[y][x].b = 1 ;	//draws square obstacle
// 					}
// 				}
// 			}
// 		}
//
// 		/*Map to SRAM*/
//     sr_map_to_mem();
// }


// void sr_map_to_mem() {
// 		/*Converts MAP to char arrays for page adressing mode*/
// 		volatile char page_data[SR_MAX_X]; 					//Contains info about an entire page
// 		volatile char col_data[OLED_PAGE_HEIGHT];	//Contains info about a single col in page
// 		for (int page = OLED_PAGES-1; page >= 0; page--) {		//Iterate from Page7 to Page0
//
// 			for (int x = 0; x < SR_MAX_X; x++ ) {		//normal iteration towards right
// 				char temp[1] = {0b00000000};
//
// 				for (int y = OLED_PAGE_HEIGHT-1; y >= 0; y--) {			//reverse iteration . start bottom left
//
// 				//	col_data[OLED_PAGE_HEIGHT - y] = MAP[page*OLED_PAGE_HEIGHT + y][x].b;			//col_data[8] = {0,1,1,0,0,1,0,1}
// 					temp[0] |= (col_data[7-y] << (y));
// 				}
// 				page_data[x] = temp[0];	//add the column to page data
// 			}
//
// 			for (int bit = 0; bit < 4; bit++) {
// 				page_adress[0] |= (page_bin[i] << 3-i);		//left shift page number //CHECK THIS
// 			}
//
// 			oled_write_c(page_adress[0]); 	//Set the page start address of the target display location by command 0xB0 to 0xB7.
// 			oled_write_c(0x00);					//Set the lower start column address
// 			oled_write_c(0x1F);					//Set the upper start column address
//
// 			oled_write_d(page_data[page]);
// 		}
//
// 		/*Test uten SRAM*/
// 		//oled_sram_adress["NEED CODE HERE"] = MAP[y][x].b;
// 		//oled_sram_adress[oled_state.LINE*128 + oled_state.COL + i] = pgm_read_byte(&font8[output][i]);
//
// }

//void sr_mem_to_oled() {}

void sr_play(Joystick* joy) {
	joystick_run(joy);
	sr_Runner* runner; // = malloc(sizeof(Runner));
	sr_Obstacle_list* obstacles; //= malloc(sizeof(Obstacle_list));
	runner 		= malloc(sizeof(sr_Runner));
	obstacles = malloc(sizeof(sr_Obstacle_list));

	sr_init(runner, obstacles);


	while (!sr_GAMEOVER) {
		sr_draw_runner(runner);
		joystick_run(joy);
		sr_run(runner, joy, obstacles);

		// if(joy->dir == UP) {
		// 	for (int i = 0; i < 30; i++) {
		// 		runner->posy -= 1;
		// 		sr_draw_runner(runner);
		// 		_delay_ms(5);
		// 	}
		// 	for (int j = 0; j < 30; j++) {
		// 		runner->posy += 1;
		// 		sr_draw_runner(runner);
		// 		_delay_ms(5);
		// 	}
		// }
		sr_GAMEOVER = 0;
	}
	free(runner);
	free(obstacles);
}
