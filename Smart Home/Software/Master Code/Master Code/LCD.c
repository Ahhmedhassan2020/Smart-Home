/*
 * LCD.c
 *
 * Created: 12/22/2022 11:21:52 AM
 *  Author: User
 */ 
#include "LCD.h"
//#include "LCD_INTERFACE.h"
void LCD_intial(void)
{
	_delay_ms(200);
	#if defined eight_bit_mode
	DIO_SET_PORT_DIR('A',1);
	DIO_SET_PIN_DIR('A',RS,1);
	DIO_SET_PIN_DIR('A',EN,1);
	LCD_send_cmd(EIGHT_BIT);
	_delay_ms(1); 
	LCD_send_cmd(CRS_ON_DSP_ON);
	 _delay_ms(1);
	LCD_send_cmd(CLR_SCREEN);
	_delay_ms(10);
	LCD_send_cmd(ENTRY_MODE);
	_delay_ms(1);
	#elif defined four_bit_mode
	DIO_SET_PIN_DIR('A',3,1);
	DIO_SET_PIN_DIR('A',4,1);
	DIO_SET_PIN_DIR('A',5,1);
	DIO_SET_PIN_DIR('A',6,1);
	DIO_SET_PIN_DIR('A',RS,1);
	DIO_SET_PIN_DIR('A',EN,1);
	LCD_send_cmd(RETURN_HOME);
	_delay_ms(10);
	LCD_send_cmd(FOUR_BIT);
	_delay_ms(1);
	LCD_send_cmd(CRS_ON_DSP_ON);
	_delay_ms(1);
	LCD_send_cmd(CLR_SCREEN);
	_delay_ms(10);
	LCD_send_cmd(ENTRY_MODE);
	_delay_ms(1);
	#endif
	
} 
/* void LCD_voidSetDirection (void)
{
	DIO_SET_PORT_DIR('A',1);
	DIO_SET_PIN_DIR ('A',RS , 1 );
	//DIO_voidSetPinDirection (LCD_RW , HIGH );
	DIO_SET_PIN_DIR ('A',EN , 1 );
}


void LCD_voidInitialize()
{
	_delay_ms(200);
	LCD_send_cmd(LCD_HOME);
	_delay_ms(10);
	LCD_send_cmd(LCD_FUNCSET_4BIT);
	_delay_ms(1);
	LCD_send_cmd(LCD_DISPON_CURSON);
	_delay_ms(1);
	LCD_send_cmd(LCD_CLR);
	_delay_ms(1);
	LCD_send_cmd(LCD_ENTRYMODE);
	_delay_ms(1);
} */
void static enable(void)
{
	DIO_WRITE_PIN('A',EN,1);
	_delay_ms(1);
	DIO_WRITE_PIN('A',EN,0);
	_delay_ms(1);
	
}
void LCD_send_char(char data)
{
	#if defined eight_bit_mode
	DIO_WRITE_PORT('A',data);
	DIO_WRITE_PIN('A',RS,1);
	enable();
	#elif defined four_bit_mode
	DIO_WRITE_PIN('A',3,READ_BIT(data,4));
	DIO_WRITE_PIN('A',4,READ_BIT(data,5));
	DIO_WRITE_PIN('A',5,READ_BIT(data,6));
	DIO_WRITE_PIN('A',6,READ_BIT(data,7));
	DIO_WRITE_PIN('A',RS,1);
	enable();
	DIO_WRITE_PIN('A',3,READ_BIT(data,0));
	DIO_WRITE_PIN('A',4,READ_BIT(data,1));
	DIO_WRITE_PIN('A',5,READ_BIT(data,2));
	DIO_WRITE_PIN('A',6,READ_BIT(data,3));
	DIO_WRITE_PIN('A',RS,1);
	enable();
	_delay_ms(1);
	#endif
}
void LCD_send_cmd(char cmd)
{
	#if defined eight_bit_mode
	DIO_WRITE_PORT('A',cmd);
	DIO_WRITE_PIN('A',RS,0);
	enable();
	_delay_ms(1);
	#elif defined four_bit_mode
	DIO_WRITE_PIN('A',3,READ_BIT(cmd,4));
	DIO_WRITE_PIN('A',4,READ_BIT(cmd,5));
	DIO_WRITE_PIN('A',5,READ_BIT(cmd,6));
	DIO_WRITE_PIN('A',6,READ_BIT(cmd,7));
	DIO_WRITE_PIN('A',RS,0);
	enable();
	DIO_WRITE_PIN('A',3,READ_BIT(cmd,0));
	DIO_WRITE_PIN('A',4,READ_BIT(cmd,1));
	DIO_WRITE_PIN('A',5,READ_BIT(cmd,2));
	DIO_WRITE_PIN('A',6,READ_BIT(cmd,3));
	DIO_WRITE_PIN('A',RS,0);
	enable();
	_delay_ms(1);
	#endif
}
void LCD_clear_screen()
{
	LCD_send_cmd(0x01);
}
void LCD_move_cursor(char row , char coloumn)
{
	char data ;
	if (row>2||row<1||coloumn<1||coloumn>16)
	{
		data = 0x80;
	}
	 if (row == 1 )
	{
		data = (0x80+coloumn-1);
	}
	else if (row == 2)
	{
		data = (0xc0+coloumn-1);
	}
	LCD_send_cmd(data);
	_delay_ms(1);
}	
void LCD_send_string(char *data)
{
	while((*data) != '\0')
	{
		LCD_send_char(*data);
		data++;
	}
}