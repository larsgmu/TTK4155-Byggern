/*!@file
* This file contains functions to convert analog signals from the game controller to digital values for the atmega162.
* The conversion is done by an ADC0844CCN chip.
*/
#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/*!
 *@brief Enum cointaining the different analog inputs from the game controller.
*/
enum channel_type{
  Y_axis        = 4, /*!< Y-axis of joystick, corresponding to UP/DOWN movement*/
  X_axis        = 5, /*!< X-axis of joystick, corresponding to LEFT/RIGH movement*/
  Right_slider  = 6,
  Left_slider   = 7,
};

/*!
*@brief Initializes ADC interface
*/
void adc_init(void);

/*!
*@brief Reads the converted signal from specified channel
*@param[in] enum channel_type channel -> Channel to read from
*@return @c uint8_t -> Digital value
*/
uint8_t adc_read(enum channel_type channel);

#endif
