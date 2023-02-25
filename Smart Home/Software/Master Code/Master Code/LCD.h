/*
 * LCD.h
 *
 * Created: 12/22/2022 12:25:58 PM
 *  Author: User
 */ 


#ifndef LCD_H_
#define LCD_H_
#include "DIO.h"
#include "LCD_config.h"
#include "std_macros.h"
#define  F_CPU 8000000UL
#include <util/delay.h>
#define CLR_SCREEN 0x01
#define RETURN_HOME 0x02
#define ENTRY_MODE 0x06
#define RETURN_HOME 0x02
#define CRS_ON_DSP_ON 0x0C
void LCD_intial(void);
void LCD_send_char(char data);
void LCD_send_cmd(char cmd);
void LCD_clear_screen();
void LCD_move_cursor(char row , char coloumn);
void LCD_send_string(char * data);
#if defined eight_bit_mode
#define RS 1
#define EN 2
#define EIGHT_BIT 0x38
#elif defined four_bit_mode
#define RS 1
#define EN 2
#define FOUR_BIT 0x28
#endif

#endif /* LCD_H_ */
