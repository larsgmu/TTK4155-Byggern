#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>

void motor_init();
void motor_run(uint8_t val);
void motor_get_position();
int16_t encoder_read();
void reset_encoder();
void solenoid();

#endif
