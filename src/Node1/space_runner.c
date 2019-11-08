#include 	"space_runner.h"
#include 	"oled_driver.h"
//#include 	<time.h>
#include 	<util/delay.h>
#include	<stdio.h>
#include  <avr/io.h>
#include	<stdlib.h>

#define SR_MAX_Y 64	//height of OLED	: 8 chars
#define SR_MAX_X 128	//width of OLED		:16 chars

static volatile char* oled_command_address 	= (char*)0x1000;
static volatile char* oled_data_address 		= (char*)0x1200;
static volatile char* oled_sram_adress 			= (char*)0x1C00;

//srand(time(0));		//seeds time

//

static uint8_t 						GRAVITY 			= 4;
static uint8_t						JUMP_SPEED		= 5;
static uint8_t 						GROUND_LEVEL 	= SR_MAX_Y-10;		//ground at 54
volatile static uint8_t 	sr_JUMPFLAG 	= 0;
volatile static uint8_t 	sr_GAMEOVER 	= 0;
volatile static uint16_t 	sr_SCORE 			= 0; 	//max is 65536

static struct oled_data_marker_struct oled_state;

/*	64x128 Game map. 1 = stuff, 0 = not stuff.	*/
//static sr_Bit MAP[SR_MAX_Y][SR_MAX_X];

void sr_init(sr_Runner* runner, sr_Obstacle_list* obst) {
		/*Initialize OLED for game purposes*/
		oled_init();

		sr_GAMEOVER = 0;
		sr_JUMPFLAG = 0;
		sr_SCORE 		= 0;

		/*Draws map and ground*/
		sr_sram_init();

    /*Init and draw runner*/
    runner->velx 	= 2;
    runner->vely 	= 0;
    runner->posy 	= GROUND_LEVEL-1;
		runner->posx 	= 5;
		sr_sprite_test(runner);		//generates runner matrix

		/*Init obstacle list*/
		obst->size = 0;
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
	/*If runner hasnt jumped, (is on ground)*/
	char temp[1] = {0b00000000};
	for (int p = OLED_PAGES-2; p >= OLED_PAGES-3; p--) { //start at p6, then p5

		uint8_t i = 0;

		//uint8_t col_data[8] = {0,0,0,0,0,0,0,0};

		for (int x = 3; x < SR_RUNNER_WIDTH+3; x++) {	//x<12
			for (int y = 0; y < OLED_PAGE_HEIGHT; y++) {	//y<8

				//col_data[y] = runner->sprite[y + i*OLED_PAGE_HEIGHT ][x].b;
				temp[0] |= ((runner->sprite[y + i*OLED_PAGE_HEIGHT ][x].b) << (7-y));
			}
			temp[0] &= ~(1 << 0x04);
			if (x == 10) {
				oled_sram_adress[p*OLED_COLS + x] = 0x00;
			}
			else {
				oled_sram_adress[p*OLED_COLS + x] = temp[0];
			}
		}
		i++;
	}
	/*If runner has jumped
	1. Draw new runner sprite in new y-position
	2. Clear out a row of pixels above and below*/

	oled_draw();
}

void sr_draw_obstacle(sr_Runner* runner, sr_Obstacle_list* o_list) {

}

//oled_sram_adress[oled_state.LINE*128 + oled_state.COL + i] = pgm_read_byte(&font8[output][i]);
void sr_sram_write(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2) {
		for (int y = y1; y < y2; y++) {
			for (int x = x1; x < x2; x++) {
				//write
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

void sr_gen_obst(sr_Obstacle_list* obst) {
	sr_Obstacle o;
	o.height = 1;
	o.posx 	= SR_MAX_X-SR_OBSTACLE_DIM;
	obst->size ++ ;
	obst->obstacles[obst->size] = o;
}

void sr_jump(sr_Runner* runner) {
    if (sr_JUMPFLAG == 1) { return;	}		//no double jumps
		sr_JUMPFLAG  = 1;
    runner->vely = JUMP_SPEED;
		/*Add another sprite*/
}


void sr_crash() {
	/*Print message to OLED */
	printf("score: %d", sr_SCORE);

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

void sr_run(sr_Runner* runner, Joystick* joy, sr_Obstacle_list* obst) {
		/*Update score*/
		sr_SCORE ++;

		/* Update Y-velocity */
		if (runner->vely != 0) { //can also check if sr_JUMPFLAG
			runner->vely -= GRAVITY*0.5;
			//If player hits ground
			if (runner->posy == GROUND_LEVEL-1 && sr_JUMPFLAG) {
				runner->vely = 0;
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
			runner->posy = (int)(runner->posy + runner->vely);		//Values probably not right
		}

		/*Update Ostacles' X-position*/
		for (int i = 0; i < obst->size; i ++) {
			obst->obstacles[i].posx -= runner->velx;

			/*Check if crash*/
			if ((runner->posx + SR_RUNNER_WIDTH <= obst->obstacles[i].posx) && (runner->posy >= GROUND_LEVEL+SR_OBSTACLE_DIM)) {
				sr_crash();
			}
		}

		/*Check if Obstacle 1 out of bounds*/
		if (obst->obstacles[0].posx <= 1) {
			for (int i = 0; i < obst->size-1; i++) {
				obst->obstacles[i] = obst->obstacles[i+1];	//Pushes the obstacles further down the queue
			}
			obst->size --;	//there might be more than (SR_DIR = space_runner;size) obstacles in list but they be ignored
		}

		/*Randomly generates obstacles, maximum 3 at a time.*/
		if (sr_SCORE%70 == 2 && obst->size < SR_OBSTACLE_NO) {
			sr_gen_obst(obst);
		}

		/*Update map*/
		sr_draw_map(runner, obst);
}

void sr_draw_map(sr_Runner* runner, sr_Obstacle_list* obst) {
    //Needs to update obstacles and stuff.
		//Ground is already drawn by init func

		/*Clear old map*/
		for (int y = 0; y < GROUND_LEVEL; y++) {
			for (int x = 0; x < SR_MAX_X; x++) {
				//MAP[y][x].b = 0;
			}
		}

		/*Draw runner if player jumped*/
		//if (sr_JUMPFLAG) {		//Might be buggy because of order of run-function
			uint8_t m = 0;
			uint8_t n = 0;
			for (int y = runner->posy - SR_RUNNER_HEIGHT; y < GROUND_LEVEL; y++) {
				for (int x = runner->posx; x < runner->posx + SR_RUNNER_WIDTH; x++) {
					//MAP[y][x].b = runner->sprite[m][n].b;
					n++ ;
				}typedef struct foo
{
    unsigned x:1;
} foo;
				m++ ;
			}
		//}

		/*Draw obstacles*/
		if (obst->size > 0) {
			for (int i = 0; i < obst->size; i++) {
				for (int y = GROUND_LEVEL-SR_OBSTACLE_DIM; y < GROUND_LEVEL; y++) {
					for (int x = obst->obstacles[i].posx; x < obst->obstacles[i].posx + SR_OBSTACLE_DIM; x++) {
						//MAP[y][x].b = 1 ;	//draws square obstacle
					}
				}
			}
		}

		/*Map to SRAM*/
    sr_map_to_mem();
}


void sr_map_to_mem() {
		/*Converts MAP to char arrays for page adressing mode*/
		volatile char page_data[SR_MAX_X]; 					//Contains info about an entire page
		volatile char col_data[OLED_PAGE_HEIGHT];	//Contains info about a single col in page
		for (int page = OLED_PAGES-1; page >= 0; page--) {		//Iterate from Page7 to Page0

			for (int x = 0; x < SR_MAX_X; x++ ) {		//normal iteration towards right
				char temp[1] = {0b00000000};

				for (int y = OLED_PAGE_HEIGHT-1; y >= 0; y--) {			//reverse iteration . start bottom left

				//	col_data[OLED_PAGE_HEIGHT - y] = MAP[page*OLED_PAGE_HEIGHT + y][x].b;			//col_data[8] = {0,1,1,0,0,1,0,1}
					temp[0] |= (col_data[7-y] << (y));
				}
				page_data[x] = temp[0];	//add the column to page data
			}

			/*Send entire page to SRAM*/		//ALTERNATIVT: en stor if/else		//ALTERNATIVT: HORIZONTAL
			uint8_t 		page_adress[1] = {0b10110000};
			uint8_t page_bin[4] = {0,0,0,0};
			uint8_t n = page;					//Convert page number to binary
			uint8_t i = 0;
			while (n >0) {
				page_bin[i] = n%2;
				n = n/2;
				i++;
			}
			for (int bit = 0; bit < 4; bit++) {
				page_adress[0] |= (page_bin[i] << 3-i);		//left shift page number //CHECK THIS
			}

			oled_write_c(page_adress[0]); 	//Set the page start address of the target display location by command 0xB0 to 0xB7.
			oled_write_c(0x00);					//Set the lower start column address
			oled_write_c(0x1F);					//Set the upper start column address

			oled_write_d(page_data[page]);
		}

		/*Test uten SRAM*/
		//oled_sram_adress["NEED CODE HERE"] = MAP[y][x].b;
		//oled_sram_adress[oled_state.LINE*128 + oled_state.COL + i] = pgm_read_byte(&font8[output][i]);

}

void sr_mem_to_oled() {

}

void sr_play(Joystick* joy) {
	sr_Runner* runner; // = malloc(sizeof(Runner));
	sr_Obstacle_list* obstacles; //= malloc(sizeof(Obstacle_list));
	runner 		= malloc(sizeof(sr_Runner));
	obstacles = malloc(sizeof(sr_Obstacle_list));

	sr_init(runner, obstacles);

	while (!sr_GAMEOVER) {
		printf("Playing Space Runner");
		_delay_ms(10);
		sr_run(runner, joy, obstacles);
	}
	printf("Score: %d", sr_SCORE);
	oled_init();
}
