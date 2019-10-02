#ifndef SLIDER_H
#define SLIDER_H

#include "adc_driver.h"

typedef struct Slider_struct {
    int right_pos;
    int left_pos;
} Slider;

void get_slider_pos(Slider* slider);

#endif
