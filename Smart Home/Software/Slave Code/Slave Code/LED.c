/*
 * LED.c
 *
 * Created: 12/3/2022 10:03:56 AM
 *  Author: Ahmed Hassan
 */ 
#include "DIO.h"
void LED_vInit(char port , char pin )
{
	DIO_SET_PIN_DIR(port , pin,1);
}
void LED_vTurnOn(char port , char pin )
{
	DIO_WRITE_PIN(port,pin,1);
}
void LED_vTurnOff(char port , char pin )
{
		DIO_WRITE_PIN(port,pin,0);
}
void LED_vToggle(char port , char pin )
{
	DIO_TOGGLE_PIN(port , pin);
}
char LED_u8ReadStatus(char port,char pin)
{
	return DIO_u8read( port, pin);//return the current status of the given pin
}