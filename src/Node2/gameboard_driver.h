/*Including servo and IR-sensor*/

#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <string.h>
#include "can_driver.h"


typedef struct Player_struct {
  string name;
  int score = 0;
  int lives = 3;
} Player;

/*Sets the servo from joystick input*/
void servo_joystick_control(CANmsg* pos_msg);

/*Initializing the internal ADC on arduioni*/
void ir_adc_init();

/*Reading the IR sensor with digital filtering*/
uint8_t ir_adc_read();

/*Checking if ball hit IR-sensor, if so, increase score*/
void ir_game_score(Player* player)

#endif
