/*!@file
* Slider interface
*/
#ifndef SLIDER_H
#define SLIDER_H

#include <stdint.h>

/*!
* @brief Slider struct containing right and left slider positions.
*/
typedef struct Slider_struct {
    uint8_t right_pos;
    uint8_t left_pos;
} Slider;

/*!
* @brief Creates and returns a slider struct with updated data.
* @return @c Slider -> Slider struct with new data.
*/
Slider slider_get();

/*!
* @brief Sends slider position CAN message.
*/
void slider_send_pos();

#endif
