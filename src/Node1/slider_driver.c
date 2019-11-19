/*!@file
* Slider interface
*/

#define F_CPU 4915200
#define SLIDER_THRESHOLD 5

#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>

#include "slider_driver.h"
#include "can_driver.h"
#include "adc_driver.h"

static uint8_t slider_left_button_prev   = 0;
static uint8_t slider_right_button_prev  = 0;
static uint8_t button                    = 0;
static uint8_t prev_slider[3]            = {0,0,0};

/*-------------------------------------------------------*/
/********Function declarations*********/

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

/*-------------------------------------------------------*/
/********Function implementations*********/

Slider slider_get() {
  Slider slider;
  slider.right_pos   = adc_read(Right_slider);
  slider.left_pos    = adc_read(Left_slider);
  return slider;
}

void slider_send_pos() {
    Slider slid = slider_get();
    CANmsg slider_msg;
    slider_msg.id     = 2;
    slider_msg.length = 3;
    button = slider_right_button_pressed();

    if ((abs(slid.right_pos-prev_slider[0])>SLIDER_THRESHOLD)
     || (abs(slid.left_pos-prev_slider[1])>SLIDER_THRESHOLD)
     || (button != prev_slider[2])) {
      slider_msg.data[0]  = slid.right_pos; // RIGHT slider
      prev_slider[0]      = slider_msg.data[0];
      slider_msg.data[1]  = slid.left_pos; // LEFT slider
      prev_slider[1]      = slider_msg.data[1];

      if (button && !prev_slider[2]) {
          slider_msg.data[2]  = 1;
          prev_slider[2]      = slider_msg.data[2];
      }
      if (!button) {
          slider_msg.data[2]  = 0;
          prev_slider[2]      = slider_msg.data[2];
      }
      can_send_msg(&slider_msg);
    }
}

uint8_t slider_left_button_pressed() {
  if (PINB&(1<<PB3)) {
    return 1;
  }
  return 0;
}

uint8_t slider_right_button_pressed() {
  if (PINB&(1<<PB2)) {
    return 1;
  }
  return 0;
}

