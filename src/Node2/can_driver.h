/*!@file
* This file contains function to enable CAN communication between Node1 and Node2.
* Node2 uses a MCP2515 chip as a stand-alone CAN controller.
* The communication between atmega2560 and MCP2515 is done with SPI.
* Node2 uses a MCP2551 chip as CAN transceiver.
*/

#ifndef CAN_H
#define CAN_H
#define CAN_MSG_DATA_LENGTH 8
#define CAN_SHIFT_CONSTANT 5
#include <avr/interrupt.h>

//extern int can_interrupt_flag;
//can_interrupt_flag = 0;

/*!
*@brief Struct containing the CAN message.
*/
typedef struct CAN_message_struct {
  uint8_t length;
  uint8_t id;
  uint8_t data[CAN_MSG_DATA_LENGTH];
} CANmsg;
/*!
*@brief Initializes CAN interace with interrupt.
*/
void can_init();

/*!
*@brief Function to send specified CAN message.
*@param[in] CANmsg* can_msg -> Pointer to CAN message to send.
*/
void can_send_msg(CANmsg* can_msg);

/*!
*@brief Function to receive CAN message when interrupt is received.
*@return @c CANmsg -> The received CAN message.
*/
CANmsg can_receive_msg();

/*!
*@brief Returns the latest received CAN message.
*@return @c CANmsg -> Latest received CAN message.
*/
CANmsg get_CAN_msg();

enum joystick_dir {
  NEUTRAL = 0,
  UP = 1,
  DOWN = 2,
  LEFT = 3,
  RIGHT = 4,
};

#endif
