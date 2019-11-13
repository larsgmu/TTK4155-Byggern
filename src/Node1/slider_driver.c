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
uint8_t slider_left_button_prev   = 0;
uint8_t slider_right_button_prev  = 0;
uint8_t prev_slider[3] = {0,0,0};
uint8_t button = 0;


void slider_run(Slider* slider) {
    slider->right_pos = adc_read(Right_slider);
    slider->left_pos = adc_read(Left_slider);
}

void send_slider_pos(Slider* slider) {

    CANmsg slider_msg;
    slider_msg.id = 2;
    slider_msg.length = 3;
    button = slider_right_button_pressed();
    if ( (abs(slider->right_pos-prev_slider[0])>SLIDER_THRESHOLD)
    || (abs(slider->left_pos-prev_slider[1])>SLIDER_THRESHOLD)
    || (button != prev_slider[2]) ){
      slider_msg.data[0] = slider->right_pos; // RIGHT slider
      prev_slider[0] = slider_msg.data[0];
      slider_msg.data[1] = slider->left_pos; // LEFT slider
      prev_slider[1] = slider_msg.data[1];

      if (button && !prev_slider[2]) {
          slider_msg.data[2] = 1;
          prev_slider[2] = slider_msg.data[2];
      }
      if (!button) {
          slider_msg.data[2] = 0;
          prev_slider[2] = slider_msg.data[2];
      }
      //printf("can_msg sendt\n\r");
      //printf("BUTTON: %d    PREV: %d\n\r", button,prev_slider[2]);
      can_send_msg(&slider_msg);
    }
}

/*Gameboard Buttons*/
uint8_t slider_left_button_pressed(){
  if((PINB&(1<<PB3)) && slider_left_button_prev==0){
    slider_left_button_prev = 1;
    return 1;
  }
  if(~(PINB&(1<<PB3))){
    slider_left_button_prev = 0;
    return 0;
  }

  return 0;
}

uint8_t slider_right_button_pressed(){
  if (PINB&(1<<PB2)) {
    return 1;
  }
  return 0;
}
