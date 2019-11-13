/*!@file
* Slider interface
*/
#ifndef SLIDER_H
#define SLIDER_H

#include "adc_driver.h"
#include "stdbool.h"
#include "stdlib.h"

typedef struct Slider_struct {
    int right_pos;
    int left_pos;
} Slider;

// void button_timer_init();
void slider_run(Slider* slider);
void send_slider_pos(Slider* slider);

/*!
*@brief Checks if left button on game controller is pressed
*@return @c 1 on success, else @c 0.
*/
uint8_t slider_left_button_pressed();

/*!
*@brief Checks if right button on game controller is pressed
*@return @c 1 on success, else @c 0.
*/
uint8_t slider_right_button_pressed();
#endif
