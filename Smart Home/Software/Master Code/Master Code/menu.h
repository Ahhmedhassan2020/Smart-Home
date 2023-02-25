/*
 * menu.h
 *
 * Created: 14/01/2023 03:28:06 م
 *  Author: User
 */ 


#ifndef MENU_H_
#define MENU_H_
#include "App_config.h"
#include "STD_Types.h"
#include "STD_MESSAGES.h"
#include "Keypad.h"
#include "SPI.h"
#include "LCD.h"
uint8 u8GetKeyPressed(uint8 u8LoginMode);
void vMenuOption(const uint8 u8SelectedRoom,const uint8 u8LoginMode);
uint8 ui8ComparePass(const uint8* pass1,const uint8* pass2,const uint8 size);




#endif /* MENU_H_ */