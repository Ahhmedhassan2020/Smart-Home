/*
 * Keypad.h
 *
 * Created: 12/25/2022 1:49:13 PM
 *  Author: User
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_
#include "DIO.h"
#define NOT_PRESSED 0xff
void keypad_vInit();char keypad_u8check_press();
#endif /* KEYPAD_H_ */