#ifndef EEPROM_H
#define EEPROM_H

#include <inttypes.h>

void 	EEPROMWrite(uint16_t uiAddress, uint8_t ucData);
uint8_t	EEPROMRead(uint16_t uiAddress);

#endif
