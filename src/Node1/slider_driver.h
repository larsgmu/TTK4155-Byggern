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

void slider_run(Slider* slider);
void send_slider_pos(Slider* slider);
#endif
