/*!@file
* Slider interface
*/
#ifndef SLIDER_H
#define SLIDER_H

#include <stdint.h>

typedef struct Slider_struct {
    uint8_t right_pos;
    uint8_t left_pos;
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
