/*!@file
* This file contains function to enable CAN communication between Node1 and Node2.
* Node2 uses a MCP2515 chip as a stand-alone CAN controller.
* The communication between atmega2560 and MCP2515 is done with SPI.
* Node2 uses a MCP2551 chip as CAN transceiver.
*/

#define CAN_SHIFT_CONSTANT 5
#define F_CPU 16000000

#include <avr/interrupt.h>
#include <util/delay.h>

#include "can_driver.h"
#include "mcp2515_driver.h"
#include "MCP2515.h"

static CANmsg latest_msg;

/*!
*@brief Function to receive CAN message when interrupt is received.
*@return @c CANmsg -> The received CAN message.
*/
CANmsg can_receive_msg();

/*-------------------------------------------------------*/
/*Function implementations*/

void can_init() {
  /*Set interrupt on PD0 to falling edge */
  EICRA &= ~(1 << ISC20);
  EICRA |= (1 << ISC21);
  /*Enable interrupt  on PD0 */
  EIMSK |= (1 << INT2);
  /*Clear interrupt flag on PD0 */
  EIFR |= (1 << INTF2);

  mcp2515_init();

  /*Normal mode*/
  mcp2515_bit_modify(MCP_CANCTRL,MODE_MASK, MODE_NORMAL);

  /*Turn mask/filters off*/
  mcp2515_write(MCP_RXB0CTRL, 0b01100000);

  /*Generate interrupt when received message kmp*/
  mcp2515_bit_modify(MCP_CANINTE, 0b00000011, MCP_RX_INT);

  /* clear interrupt flag*/
  mcp2515_bit_modify(MCP_CANINTF, 0b00000011, 0);

  latest_msg.id = 6;
  latest_msg.data[0] = 8;
}

void can_send_msg(CANmsg* can_msg) {

  mcp2515_write(MCP_TXB0SIDH, 0);
  mcp2515_write(MCP_TXB0SIDL, can_msg->id << CAN_SHIFT_CONSTANT);
  mcp2515_write(MCP_TXB0DLC, can_msg->length);

  for (int length = 0; length < can_msg->length; length++){
    mcp2515_write(MCP_TXB0D0 + length, can_msg->data[length]);
    _delay_ms(1);
  }
  mcp2515_request_send(0);
}

CANmsg can_receive_msg() {
  CANmsg msg;
  msg.length = mcp2515_read(MCP_RXB0DLC);
  msg.id = ( (mcp2515_read(MCP_RXB0SIDH) >> 3) + (mcp2515_read(MCP_RXB0SIDL) >> CAN_SHIFT_CONSTANT) );
  for (int i = 0; i < msg.length; i++){
    msg.data[i] = mcp2515_read(MCP_RXB0D0 + i);
  }
  mcp2515_bit_modify(MCP_CANINTF, 0b00000001, 0);
  return msg;
}

CANmsg get_CAN_msg(){
    return latest_msg;
}

ISR(INT2_vect){
 latest_msg = can_receive_msg();
}
