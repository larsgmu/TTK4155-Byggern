#ifndef EEPROM_DRIVER_H
#define EEPROM_DRIVER_H
#define HIGHSCORE_PINGPONG_ADDR 0
#define HIGHSCORE_SRL_ADDR 100
#define HIGHSCORE_SRH_ADDR 200

/*!
* @brief Writes to EEPROM.
* @param[in] @c unsigned int ui_adress -> Address to write to.
* @param[in] @c unsigned char uc_data -> Data written to address.
*/
void eeprom_write(unsigned int ui_address, unsigned char uc_data);

/*!
* @brief Reads from EEPROM.
* @param[in] @c unsigned int ui_adress -> Address read from.
* @return unsigned char -> Data at address read from.
*/
unsigned char eeprom_read(unsigned int ui_address);



#endif
