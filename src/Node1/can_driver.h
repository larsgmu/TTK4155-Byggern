#ifndef CAN_H
#define CAN_H

#define CAN_MSG_DATA_LENGTH 8

typedef struct CAN_message_struct {
  uint8_t length;
  uint8_t id;
  uint8_t data[CAN_MSG_DATA_LENGTH];
} CANmsg;



void can_init();
void can_send_msg(CANmsg can_msg);
CANmsg can_recieve_msg();

#endif
