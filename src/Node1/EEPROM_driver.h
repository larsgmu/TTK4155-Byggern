#ifndef EEPROM_DRIVER_H
#define EEPROM_DRIVER_H
#define HIGHSCORE_PINGPONG_ADDR 0
#define HIGHSCORE_SRL_ADDR 100
#define HIGHSCORE_SRH_ADDR 200


void EEPROM_write(unsigned int uiAddress, unsigned char ucData);

unsigned char EEPROM_read(unsigned int uiAddress);



#endif
