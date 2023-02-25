/*
 * EEPROM.h
 *
 * Created: 12/12/2022 2:47:53 PM
 *  Author: Ahmed Hassan
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
#include "STD_Types.h"
#include <avr/io.h>
#include "std_macros.h"
void EEPROM_write(unsigned short address , unsigned char data);
unsigned char EEPROM_read(unsigned short address);

#endif /* EEPROM_H_ */