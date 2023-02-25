/*
 * DIO.h
 *
 * Created: 12/1/2022 11:21:42 PM
 *  Author: Ahmed Hassan
 */ 


#ifndef DIO_H_
#define DIO_H_
#include "DIO_config_master.h"
void DIO_SET_PIN_DIR(char port , char pin , char dir);
void DIO_WRITE_PIN(char port , char pin , char val);
void DIO_TOGGLE_PIN(char port , char pin );
unsigned char DIO_U8READ_PIN(char port , char pin );
void DIO_SET_PORT_DIR(char port , char dir);
void DIO_WRITE_PORT(char port , char val);
void DIO_TOGGLE_PORT(char port );
unsigned char DIO_U8READ_PORT(char port );
void DIO_venable_Pullup(char port , char pin , char status);






#endif /* DIO_H_ */