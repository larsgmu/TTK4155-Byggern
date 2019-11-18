#define F_CPU 4915200

#define SR_MAX_Y 64		//height of OLED	: 8 chars
#define SR_MAX_X 128	//width of OLED		:16 chars
#define SR_GROUND_LEVEL 56

#include 	<util/delay.h>
#include	<stdlib.h>
#include 	<stdint.h>

#include "slider_driver.h"
#include "joystick_driver.h"
#include 	"space_runner.h"
#include 	"oled_driver.h"

/*todo:
runner sr_sprite
progmem shit
init oled message
*/

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

static volatile char* oled_sram_adress 	= (char*)0x1C00;

static uint8_t 						GRAVITY       = 4;
static uint8_t						JUMP_SPEED		= 5;
static uint8_t						RUNNER_SPEED  = 2;
volatile static uint8_t 	sr_JUMPFLAG   = 0;
volatile static uint8_t 	sr_GAMEOVER   = 0;
volatile static uint16_t 	sr_SCORE 		  = 0; 	//max is 65536


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

void sr_sprite_init(sr_Runner* runner) {
	for (int y = 0; y < SR_RUNNER_HEIGHT; y++) {
	 	for (int x = 0; x < SR_RUNNER_WIDTH; x++) {
	 			runner->sprite[y][x] = 1;
	 	}
	}
  {   //Hard coded sprite
    runner->sprite[0][0]  = 0;
    runner->sprite[0][1]  = 0;
    runner->sprite[0][2]  = 0;
    runner->sprite[0][3]  = 0;
    runner->sprite[1][3]  = 0;
    runner->sprite[1][5]  = 0;
    runner->sprite[2][0]  = 0;
    runner->sprite[2][1]  = 0;
    runner->sprite[2][3]  = 0;
    runner->sprite[2][5]  = 0;
    runner->sprite[3][0]  = 0;
    runner->sprite[3][1]  = 0;
    runner->sprite[3][3]  = 0;
    runner->sprite[3][5]  = 0;
    runner->sprite[4][0]  = 0;
    runner->sprite[4][1]  = 0;
    runner->sprite[4][5]  = 0;
    runner->sprite[5][1]  = 0;
    runner->sprite[5][4]  = 0;
    runner->sprite[5][5]  = 0;
    runner->sprite[6][5]  = 0;
    runner->sprite[7][0]  = 0;
    runner->sprite[7][1]  = 0;
    runner->sprite[7][4]  = 0;
    runner->sprite[8][0]  = 0;
    runner->sprite[8][1]  = 0;
    runner->sprite[8][4]  = 0;
    runner->sprite[8][5]  = 0;
    runner->sprite[9][0]  = 0;
    runner->sprite[9][5]  = 0;
    runner->sprite[11][1] = 0;
    runner->sprite[11][4] = 0;
    runner->sprite[12][1] = 0;
    runner->sprite[12][4] = 0;
    runner->sprite[15][0] = 0;
    runner->sprite[15][5] = 0;
  }
}

void sr_init(sr_Runner* runner, sr_Obstacle_list* o_list) {
		/*Initialize OLED for game purposes*/
		oled_init();
    /*Reset flags and counters*/
		sr_GAMEOVER = 0;
		sr_JUMPFLAG = 0;
		sr_SCORE    = 0;

		/*Draws map and ground*/
		sr_sram_init();

		/*Init runner*/
  	runner->velx 	= RUNNER_SPEED;
  	runner->vely 	= 0;
  	runner->posy 	= SR_GROUND_LEVEL-1;
		runner->posx 	= 10;
		sr_sprite_init(runner);		//generates runner matrix

		/*Init obstacle list*/
		o_list->size = 0;

    /*Init a random seed for generating obstacles*/
    uint16_t seed = rand();
    srand(seed);
}

void sr_play(char* diff) {
	sr_Runner* 			runner;
	sr_Obstacle_list* 	o_list;
	runner 		= malloc(sizeof(sr_Runner));
	o_list 		= malloc(sizeof(sr_Obstacle_list));

	sr_init(diff, runner, o_list);

	while (!sr_GAMEOVER) {
		sr_run(runner, o_list);
	}
	free(runner);
	free(o_list);
	oled_init();
}

void sr_run(sr_Runner* runner, sr_Obstacle_list* o_list) {
		/*Update score*/
		sr_SCORE += (int)(runner->velx / 2);

		/* Update Y-velocity */
		if (runner->vely != 0) {
			runner->vely -= GRAVITY*0.1;

			/*If player hits ground*/
			if (runner->posy >= SR_GROUND_LEVEL-1 && sr_JUMPFLAG) {
				runner->vely = 0;
				runner->posy = SR_GROUND_LEVEL-1;
				sr_JUMPFLAG = 0;
				/*Change back to running sprite*/
        {
          runner->sprite[0][0] = 0;
          runner->sprite[0][4] = 1;
          runner->sprite[1][0] = 1;
          runner->sprite[1][1] = 1;
          runner->sprite[1][2] = 1;
          runner->sprite[1][4] = 1;
          runner->sprite[1][5] = 0;
          runner->sprite[2][0] = 0;
          runner->sprite[2][2] = 1;
          runner->sprite[2][4] = 1;
          runner->sprite[2][5] = 0;
          runner->sprite[3][0] = 0;
          runner->sprite[3][1] = 0;
          runner->sprite[3][2] = 1;
          runner->sprite[3][5] = 0;
          runner->sprite[4][1] = 0;
          runner->sprite[5][0] = 1;
          runner->sprite[6][0] = 1;
          runner->sprite[7][0] = 0;
        }
			}
		}
    else { /* Update X-velocity from right slider */
      Slider slider = slider_get();
      if (slider.right_pos < 90) {
        runner->velx = 2;
      }
      else if (slider.right_pos < 160 ) {
        runner->velx = 3;
      }
      else if (slider.right_pos < 230) {
        runner->velx = 4;
      }
      else {
        runner->velx = 6;
      }
    }

		/*Jump*/
		if (joystick_get_direction() == UP) {
			sr_jump(runner);
		}

		/*Update Y-position*/
		if (sr_JUMPFLAG) {
			runner->posy = (int)(runner->posy - runner->vely);
		}

		/*Update Ostacles' X-position*/
		for (int i = 0; i < o_list->size; i ++) {
			o_list->obstacles[i].posx -= runner->velx;

			/*Check if crash*/
			if ((o_list->obstacles[i].posx <= SR_RUNNER_WIDTH + runner->posx-1) &&
				(o_list->obstacles[i].posx + SR_OBSTACLE_DIM >= runner->posx) &&
				(runner->posy > SR_GROUND_LEVEL-8)) {
					sr_crash();
			}
		}

		// /*Check if Obstacle 1 out of bounds*/
		if (o_list->obstacles[0].posx <= runner->velx) {
			if (o_list->size > 1) {
				for (int o = 0; o < (o_list->size)-1; o++) {
					o_list->obstacles[o] = o_list->obstacles[o+1];
				}
			}
			if (o_list->size >= 1) {
				o_list->size --; //dequeue
			}
			for (int h = 0; h < 2; h++) {
				for (int x = 0; x < SR_OBSTACLE_DIM + runner->posx; x++) {
		 			oled_sram_adress[(6-h)*OLED_COLS + x] = 0x00;
		 		}
			}

		}

		/*Randomly generates obstacles, maximum 2 in the frame at a time.*/
		if  ((rand() % (180/runner->velx) == 0 || rand() % (182/runner->velx) == 0) && o_list->size < SR_OBSTACLE_NO) {
      srand(rand()); //new seed for continuously random spawning
			sr_gen_obstacle(o_list);
		}

		/*Update map*/
		sr_draw_runner(runner);
		sr_draw_obstacle(runner, o_list);
		_delay_ms(16); //60 fps
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
				for (int x = (runner->posx); x < SR_RUNNER_WIDTH+(runner->posx); x++) {
					for (int y = 0; y < OLED_PAGE_HEIGHT; y++) {
            /*Draw the sprite with 0's and 1's*/
						if ((runner->sprite[y + i*OLED_PAGE_HEIGHT ][x-(runner->posx)] == 1)) {
							temp[0] |=  (1 << (7-y));  //shift the 1's into place
						}
						else {
							temp[0] &= ~(1 << (7-y)); //shift the 0's into place
						}
					}
          /*Write 8 bits to SRAM*/
					oled_sram_adress[p*OLED_COLS + x] = temp[0];
					/*Clear page above Runner*/
					oled_sram_adress[4*OLED_COLS + x] = 0x00;
				}
				i++;
			}
		}

		/*If runner has jumped. Sprite takes up 2 or 3 pages.*/
		else if (sr_JUMPFLAG == 1){
			bottom_page 	=  runner->posy / OLED_PAGE_HEIGHT ;
			top_page 			= (runner->posy - SR_RUNNER_HEIGHT +1 ) / OLED_PAGE_HEIGHT ;
			temp[0] 			= 0b00000000;
			i 						= 0;
			/*Difference between bottom of page and runner y-pos*/
			uint8_t diff 	= (bottom_page+1)*OLED_PAGE_HEIGHT - runner->posy - 1;

			/*Bottom range: 1<p<6 . Top range: 0<p<5 */
			for (int p = bottom_page; p >= top_page; p--) {
				for (int x = (runner->posx); x < SR_RUNNER_WIDTH + (runner->posx); x++) {
					for (int y = 0; y < OLED_PAGE_HEIGHT; y++) {

						/*If runner is perfectly within 2 pages*/
						if ((runner->posy + 1) % 8 == 0) {
							if ((runner->sprite[y + i*OLED_PAGE_HEIGHT ][x-(runner->posx)] == 1)) {
								temp[0] |=  (1 << (7-y));	//shift the 1's into place
							}
							else {
								temp[0] &= ~(1 << (7-y)); //shift the 0's into place
							}
							/*Clear out pages above runner*/
							if (p > 1) {
								oled_sram_adress[(p-1)*OLED_COLS + x] = 0x00;
								oled_sram_adress[(p-2)*OLED_COLS + x] = 0x00;
								}
						}

            /*If runner is between 3 pages*/
						else  {
							if (p == bottom_page) {		/*This page has bottom of sprite*/
								if (runner->sprite[y][x-(runner->posx)] == 1) {
									temp[0] |=  (1 << (7-y-diff));
								}
								else {
									temp[0] &= ~(1 << (7-y-diff));
								}
							}

							else if (p == top_page) { /*This page has the top of the sprite*/
								uint8_t ind = y + 2*OLED_PAGE_HEIGHT-diff;
								if (ind < SR_RUNNER_HEIGHT) {		/*If the runner is actually at the y-position we iterate through*/
									if (runner->sprite[ind][x-(runner->posx)] == 1) {
										temp[0] |=  (1 << (7-y)); //Set pixels in sprite
									}
									else {
										temp[0] &= ~(1 << (7-y)); //Clear pixels in sprite
									}
								}
								else { 	/*If there is no runner there, clear pixels0*/
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
          /*Write column data to SRAM*/
          oled_sram_adress[p*OLED_COLS + x] = temp[0];
					temp[0] 			= 0b00000000; //reset
					if(bottom_page < 6) { /*Clear pages below runner*/
						oled_sram_adress[(bottom_page+1)*OLED_COLS+x] = 0x00;
					}
				}
				i++;
			}
		}
}

void sr_gen_obstacle(sr_Obstacle_list* o_list) {
	sr_Obstacle obstacle ;
	if (rand() % 2 == 1) {
		obstacle.height = 2;
	}
	else 	{
    obstacle.height = 1;
	}
	obstacle.posx 	= SR_MAX_X;
	o_list->size ++ ;
	o_list->obstacles[(o_list->size)-1] = obstacle;
}

void sr_draw_obstacle(sr_Runner* runner, sr_Obstacle_list* o_list) {
    uint8_t page = 6;
  	/*Iterate for every obstacle in list*/
  	for (int o = 0; o < o_list->size; o++) {
		uint8_t height = o_list->obstacles[o].height;

	  /*For every height in obstacle; 1 or 2 */
		for (int h = 0; h < height; h++) {

			/*For every column in obstacle*/
			for (int x = o_list->obstacles[o].posx; x < o_list->obstacles[o].posx + SR_OBSTACLE_DIM; x++) {
				if (x >= 0 && x < SR_MAX_X) { //if within bounds
					if (h == height-1) {	//draw a spike shape
						oled_sram_adress[(page-h)*OLED_COLS + x] = obstacle_spike[x - o_list->obstacles[0].posx];
					}
					else { //Square 8x8 Obstacle
						oled_sram_adress[(page-h)*OLED_COLS + x] = 0xFF;
					}

				/*Clear the old obstable position columns, depending on runner speed*/
				if (x < SR_MAX_X - runner->velx) {
					for (int c = 0; c < runner->velx; c++) {
						oled_sram_adress[(page-h)*OLED_COLS + x + c + 1] = 0x00;
						}
					}
				}
			}
		}
	}
}

void sr_jump(sr_Runner* runner) {
    if (sr_JUMPFLAG == 1) { return;	} //No double jumps
		sr_JUMPFLAG  = 1;
    runner->vely = JUMP_SPEED;

		/*Change sprite to jumping*/
    {
      runner->sprite[0][0] = 1;
      runner->sprite[0][4] = 0;
      runner->sprite[1][0] = 1;
      runner->sprite[1][1] = 0;
      runner->sprite[1][2] = 0;
      runner->sprite[1][4] = 0;
      runner->sprite[1][5] = 1;
      runner->sprite[2][0] = 1;
      runner->sprite[2][2] = 0;
      runner->sprite[2][4] = 0;
      runner->sprite[2][5] = 1;
      runner->sprite[3][0] = 1;
      runner->sprite[3][1] = 1;
      runner->sprite[3][2] = 0;
      runner->sprite[3][5] = 1;
      runner->sprite[4][1] = 1;
      runner->sprite[5][0] = 0;
      runner->sprite[6][0] = 0;
      runner->sprite[7][0] = 1;
    }
}

void sr_crash() {
	/*Print message to OLED */
	oled_pos(3,20);
	char score[5];
  itoa(sr_SCORE, score, 10);
	oled_sram_write_string("Score: ");
	oled_sram_write_string(score);
	_delay_ms(1000);

	/*Draws OLED white, a few columns at a time*/
	for (int p = 0; p < OLED_PAGES; p++) {
		if (p != 3) {
			for (int x = 3; x < OLED_COLS+3; x += 4) {
				oled_sram_adress[p*OLED_COLS + x]     = 0xFF;
				oled_sram_adress[p*OLED_COLS + x - 1] = 0xFF;
				oled_sram_adress[p*OLED_COLS + x - 2] = 0xFF;
				oled_sram_adress[p*OLED_COLS + x - 3] = 0xFF;
				oled_draw();
			}
		}
	}
	_delay_ms(500);
	for (int x = 3; x < OLED_COLS+3; x += 4) {
		oled_sram_adress[3*OLED_COLS + x]     = 0xFF;
		oled_sram_adress[3*OLED_COLS + x - 1] = 0xFF;
		oled_sram_adress[3*OLED_COLS + x - 2] = 0xFF;
		oled_sram_adress[3*OLED_COLS + x - 3] = 0xFF;
		oled_draw();
	}
	_delay_ms(100);
	sr_GAMEOVER = 1;
}
