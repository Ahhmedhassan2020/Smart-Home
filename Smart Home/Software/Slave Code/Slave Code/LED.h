/*
 * LED.h
 *
 * Created: 12/3/2022 10:09:39 AM
 *  Author: User
 */ 


#ifndef LED_H_
#define LED_H_
void LED_vInit(char port , char pin );
void LED_vTurnOn(char port , char pin );
void LED_vTurnOff(char port , char pin);
void LED_vToggle(char port , char pin );
char LED_u8ReadStatus(char port,char pin);




#endif /* LED_H_ */