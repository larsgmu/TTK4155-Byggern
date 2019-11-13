#define F_CPU 4915200

#include 	"space_runner.h"
#include 	"oled_driver.h"
//#include <time.h>
#include 	<util/delay.h>
#include	<stdio.h>
#include  <avr/io.h>
#include	<stdlib.h>
#include 	<stdint.h>

#define SR_MAX_Y 64		//height of OLED	: 8 chars
#define SR_MAX_X 128	//width of OLED		:16 chars

//static const int8_t O_OFFSET = -8

const uint8_t obstacle_spike [8]  = {
  0b11000000,
  0b11110000,
  0b11111100,
  0b11111111,
  0b11111111,
  0b11111100,
  0b11110000,
  0b11000000,
};

static volatile char* oled_command_address 	= (char*)0x1000;
static volatile char* oled_data_address 	= (char*)0x1200;
static volatile char* oled_sram_adress 		= (char*)0x1C00;

static uint8_t 						GRAVITY 		= 4;
static uint8_t						JUMP_SPEED		= 5;
static uint8_t						RUNNER_SPEED	= 2;
static uint8_t 						GROUND_LEVEL 	= SR_MAX_Y-8;		//ground at 56
volatile static uint8_t 	sr_JUMPFLAG 	= 0;
volatile static uint8_t 	sr_GAMEOVER 	= 0;
volatile static uint16_t 	sr_SCORE 		= 0; 	//max is 65536

void sr_init(char* diff, sr_Runner* runner, sr_Obstacle_list* o_list) {
		/*Initialize OLED for game purposes*/
		oled_init();

		sr_GAMEOVER = 0;
		sr_JUMPFLAG = 0;
		sr_SCORE	= 0;

		/*Draws map and ground*/
		sr_sram_init();

		/*Init runner*/
		if 	 (diff == "HARD") {
			RUNNER_SPEED = 5;
		}
		else {
			RUNNER_SPEED = 2;
		}
    	runner->velx 	= RUNNER_SPEED;
    	runner->vely 	= 0;
    	runner->posy 	= GROUND_LEVEL-1;
		runner->posx 	= 10;
		sr_sprite_test(runner);		//generates runner matrix

		/*Init obstacle list*/
		o_list->size = 0;
}

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
		uint8_t bottom_page = OLED_PAGES - 2 ;  //Page containing bottom of sprite
		uint8_t top_page 		= OLED_PAGES - 3 ;  //Page contains top/head of sprite
		char 		temp[1] 		= {0b00000000};	//Contains info about 8 pixels; 1 col in 1 page
		uint8_t i 					= 0;


		/*If runner hasnt jumped, (is on ground)*/
		if (sr_JUMPFLAG == 0) {
			for (int p = bottom_page; p >= top_page; p--) { //start at p6, then p5
				for (int x = (runner->posx); x < SR_RUNNER_WIDTH+(runner->posx); x++) {	//x<12
					for (int y = 0; y < OLED_PAGE_HEIGHT; y++) {	//y<8
						if ((runner->sprite[y + i*OLED_PAGE_HEIGHT ][x-(runner->posx)] == 1)) {
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
          //_delay_ms(10);
					for (int y = 0; y < OLED_PAGE_HEIGHT; y++) {

						/*If runner is perfectly within 2 pages*/
						if ((runner->posy + 1) % 8 == 0) {
							if ((runner->sprite[y + i*OLED_PAGE_HEIGHT ][x-(runner->posx)] == 1)) {
								temp[0] |=  (1 << (7-y));	//Light up pixel
							}
							else {
								temp[0] &= ~(1 << (7-y)); //Not light up pixel
							}
							/*Clear out page below and above runner*/
							if (p > 1) {
								oled_sram_adress[(p-1)*OLED_COLS + x] = 0x00;	//above
								oled_sram_adress[(p-2)*OLED_COLS + x] = 0x00;	//above
								}
							oled_sram_adress[(6)*OLED_COLS + x] = 0x00;	//below //not optimal

						}

						else  { /*If runner is between 3 pages*/
							if (p == bottom_page) {		/*This page has bottom of sprite*/
								if (runner->sprite[y][x-(runner->posx)] == 1) {
									temp[0] |=  (1 << (7-y-diff));
								}
								else {
									temp[0] &= ~(1 << (7-y-diff));
								}
								if (y < diff) { 		//Should clear rest of bits below runner BUT DOESNT !!! ARHGHGH
									temp[0] &= ~(1 << (7-(diff)+y+1));
								}

								//temp[0] &= ~(1 << (7-diff+1));		//clearing old line below runner
								//if (p < 5) { /*Clear page above runner*/
								//	oled_sram_adress[(p-1)*OLED_COLS + x] = 0x00;
								//}
							}
							else if (p == top_page) { /*This page has the top of the sprite*/
								uint8_t ind = y + 2*OLED_PAGE_HEIGHT-diff;
								if (ind < SR_RUNNER_HEIGHT) {		/*If runner sprite is actually in MAP y-pos*/
									if (runner->sprite[ind][x-(runner->posx)] == 1) {
										temp[0] |=  (1 << (7-y));
									}
									else {
										temp[0] &= ~(1 << (7-y));
									}
								}
								else { 	/*Set 0 to the rest of page-column*/
									temp[0] &= ~(1 << (7-y));
								}
								if (p >1) { /*Clear page above runner*/
									oled_sram_adress[(p-1)*OLED_COLS + x] = 0x00;
								}
							}
							else { /* This page has the middle of the sprite */
								if (runner->sprite[7-diff+y][x-(runner->posx)] == 1) {
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
					if(bottom_page < 5) {
						oled_sram_adress[(bottom_page+1)*OLED_COLS+x] = 0x00;
					}
				}
				i++;
			}

		}
		//oled_draw();
}

void sr_gen_obst(sr_Obstacle_list* o_list) {
	sr_Obstacle obstacle ;
	if (rand() % 6 == 5) {   //CHECK THIS
		obstacle.height = 2;
	}
	else 	{	//TODO
	}

	obstacle.height = 2;
	obstacle.posx 	= SR_MAX_X;// - O_OFFSET;
	/*add obstacle to list*/
	o_list->size ++ ;
	o_list->obstacles[(o_list->size)-1] = obstacle;
}

void sr_draw_obstacle(sr_Runner* runner, sr_Obstacle_list* o_list) {
	uint8_t page = 6;
  	/*Iterate for every obstacle in list*/
  	for (int o = 0; o < o_list->size; o++) {
		uint8_t height = o_list->obstacles[o].height;
		//printf("height: %d \n\r", height);
	  /*For every height in obstacle; 1 or 2 */
		for (int h = 0; h < height; h++) {		//o_list->obstacles[o].height ; h++) { WTFFF
			/*For every column in obstacle*/
			for (int x = o_list->obstacles[o].posx; x < o_list->obstacles[o].posx + SR_OBSTACLE_DIM; x++) {
				if (x >= 0 && x < SR_MAX_X) {
					if (h == height-1) {	//draw a spike shape
						oled_sram_adress[(page-h)*OLED_COLS + x] = obstacle_spike[x - o_list->obstacles[0].posx];
					}
					else { //Square 8x8 Obstacle
						oled_sram_adress[(page-h)*OLED_COLS + x] = 0xFF;
					}

				//clears previous columns, how many depends on x-velocity
				if (x < SR_MAX_X - runner->velx) {
					for (int c = 0; c < runner->velx; c++) {
						oled_sram_adress[(page-h)*OLED_COLS + x + c + 1] = 0x00;
						}
					}
				}
			}
		}
	}
	//for (int x = 0; x < OLED_COLS; x++) {
	//	oled_sram_adress[7*OLED_COLS + x] = 0b01001011;
	//}
}


void sr_sprite_test(sr_Runner* runner) {			//makes a square runner
	for (int y = 0; y < SR_RUNNER_HEIGHT; y++) {
		for (int x = 0; x < SR_RUNNER_WIDTH; x++) {
				runner->sprite[y][x] = 1;
		}
	}
}

void sr_jump(sr_Runner* runner) {
    if (sr_JUMPFLAG == 1) { return;	} //no double jumps
		sr_JUMPFLAG  = 1;
    runner->vely = JUMP_SPEED;
		/*Add another sprite*/
}


void sr_crash() {

	/*Print message to OLED */
	oled_pos(3,20);
	char score[5];
  	itoa(sr_SCORE, score, 10);
	// uint8_t i = 0;
	// while(sr_SCORE) {
	// 	char c = (sr_SCORE % 10) + '0';
	// 	score[4-i] = c;
	// 	sr_SCORE = (sr_SCORE - sr_SCORE % 10)/10 ;
	// 	i++;
	// }
	printf("Something: %s", score);
	oled_sram_write_string("Score: ");
	oled_sram_write_string(score);
	_delay_ms(1000);
	/*Draws OLED white*/
	for (int p = 0; p < OLED_PAGES; p++) {
		if (p != 3) {
			for (int x = 3; x < OLED_COLS+3; x += 4) {
				oled_sram_adress[p*OLED_COLS + x] = 0xFF;
				oled_sram_adress[p*OLED_COLS + x - 1] = 0xFF;
				oled_sram_adress[p*OLED_COLS + x - 2] = 0xFF;
				oled_sram_adress[p*OLED_COLS + x - 3] = 0xFF;
				//_delay_us(1);
				oled_draw();
			}
		}
	}
	_delay_ms(500);
	for (int x = 3; x < OLED_COLS+3; x += 4) {
		oled_sram_adress[3*OLED_COLS + x] = 0xFF;
		oled_sram_adress[3*OLED_COLS + x - 1] = 0xFF;
		oled_sram_adress[3*OLED_COLS + x - 2] = 0xFF;
		oled_sram_adress[3*OLED_COLS + x - 3] = 0xFF;
		//_delay_us(1);
		oled_draw();
	}
	_delay_ms(100);
	sr_GAMEOVER = 1;
}

void sr_run(sr_Runner* runner, Joystick* joy, sr_Obstacle_list* o_list) {
		/*Update score*/
		sr_SCORE ++;

		/*generates a random function for obstacle generate*/
		uint16_t seed = rand();
		srand(seed);

		/* Update Y-velocity */
		if (runner->vely != 0) {
			runner->vely -= GRAVITY*0.1;
			/*If player hits ground*/
			if (runner->posy >= GROUND_LEVEL-1 && sr_JUMPFLAG) {
				runner->vely = 0;
				runner->posy = GROUND_LEVEL-1;
				sr_JUMPFLAG = 0;
				/*Change back to original sprite*/
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
			//if ((runner->posx + SR_RUNNER_WIDTH <= o_list->obstacles[i].posx) && (runner->posy >= GROUND_LEVEL-SR_OBSTACLE_DIM)) {
			if ((o_list->obstacles[i].posx <= SR_RUNNER_WIDTH + runner->posx) &&
				(o_list->obstacles[i].posx + SR_OBSTACLE_DIM >= runner->posx) &&
				(runner->posy > GROUND_LEVEL-8)) {
					sr_crash();
					//printf("CRASH\n" );
			}
		}

		// /*Check if Obstacle 1 out of bounds*/
		if (o_list->obstacles[0].posx <= runner->velx) { //O_OFFSET) {
			if (o_list->size > 1) {
				for (int o = 0; o < (o_list->size)-1; o++) {
					o_list->obstacles[o] = o_list->obstacles[o+1];
				}
			}
			if (o_list->size >= 1) {
				o_list->size --; //dequeue
			}
			for (int h = 0; h < 2; h++) {		//CHECK THIS
				for (int x = 0; x < SR_OBSTACLE_DIM + runner->posx; x++) {//runner->posx + SR_RUNNER_WIDTH; x++) {
		 			oled_sram_adress[(6-h)*OLED_COLS + x] = 0x00;		//see if it works without this
		 		}
			}

		}

		/*Randomly generates obstacles, maximum 3 at a time.*/
		if  (sr_SCORE % 20 == 0  && o_list->size < SR_OBSTACLE_NO) {
			//((rand() % 500) == 5 && o_list->size < SR_OBSTACLE_NO) {
			sr_gen_obst(o_list);
		}

		/*Update map*/
		sr_draw_runner(runner);
		sr_draw_obstacle(runner, o_list);
		_delay_ms(5);
		oled_draw();

}

void sr_play(char* diff, Joystick* joy, Slider* slider) {
	joystick_run(joy);
	sr_Runner* 			runner;
	sr_Obstacle_list* 	o_list;
	runner 		= malloc(sizeof(sr_Runner));
	o_list 		= malloc(sizeof(sr_Obstacle_list));

	sr_init(diff, runner, o_list);

	while (!sr_GAMEOVER) {
		sr_draw_runner(runner);
		joystick_run(joy);
		sr_run(runner, joy, o_list);
		/*if(joy->dir == UP) {
			for (int i = 0; i < 30; i++) {
				runner->posy -= 1;
				sr_draw_runner(runner);
				_delay_ms(5);
			}
			for (int j = 0; j < 30; j++) {
				runner->posy += 1;
				sr_draw_runner(runner);
				_delay_ms(5);
			}
		}*/

		//sr_GAMEOVER = 0;
	}
	free(runner);
	free(o_list);
	oled_init();
}
