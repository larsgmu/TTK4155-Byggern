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

Slider slider_get();

void send_slider_pos();

#endif
