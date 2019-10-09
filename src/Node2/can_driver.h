/*!@file
* CAN interface
*/
#ifndef CAN_H
#define CAN_H
#define CAN_MSG_DATA_LENGTH 8
#define CAN_SHIFT_CONSTANT 5
#include <avr/interrupt.h>

//extern int can_interrupt_flag;
//can_interrupt_flag = 0;

typedef struct CAN_message_struct {
  uint8_t length;
  uint8_t id;
  uint8_t data[CAN_MSG_DATA_LENGTH];
} CANmsg;

void can_init();
void can_send_msg(CANmsg* can_msg);
CANmsg can_receive_msg();

enum joystick_dir {
  NEUTRAL = 0,
  UP = 1,
  DOWN = 2,
  LEFT = 3,
  RIGHT = 4,
};

#endif
