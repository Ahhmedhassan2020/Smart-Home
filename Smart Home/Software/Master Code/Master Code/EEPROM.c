/*
 * EEPROM.c
 *
 * Created: 12/12/2022 2:19:45 PM
 *  Author: Ahmed Hassan
 */ 

#include "EEPROM.h"
void EEPROM_write(unsigned short address , unsigned char data)
{
	EEAR = address ; 
	EEDR = data ; 
	SET_BIT(EECR,EEMWE);
	SET_BIT(EECR,EEWE);
	while(READ_BIT(EECR,EEWE)==1);	
}
unsigned char EEPROM_read(unsigned short address)
{
	EEAR = address ; 
	SET_BIT(EECR,EERE);
	return EEDR ;
}
