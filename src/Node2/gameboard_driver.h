/*Including servo and IR-sensor*/

#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include "can_driver.h"


void servo_joystick_control(CANmsg* pos_msg);

/*Initializing the internal ADC on arduioni*/
void ir_adc_init();
uint8_t ir_adc_read();

#endif
