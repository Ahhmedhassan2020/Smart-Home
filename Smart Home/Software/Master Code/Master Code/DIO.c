/*
 * DIO.c
 *
 * Created: 12/1/2022 10:35:53 PM
 * Author : Ahmed Hassan 
 */ 

#include <avr/io.h>
#include "std_macros.h"
void DIO_SET_PIN_DIR(char port , char pin , char dir)
{
switch(port)
{
	case 'A':
	case 'a':
	if (dir == 1 )
	{
		SET_BIT(DDRA,pin);
	}
	else
	{
		CLR_BIT(DDRA,pin);
	}
	break;
	case 'B':
	case 'b':
	if (dir == 1 )
	{
		SET_BIT(DDRB,pin);
	}
	else
	{
		CLR_BIT(DDRB,pin);
	}
	break;
	case 'C':
	case 'c':
	if (dir == 1 )
	{
		SET_BIT(DDRC,pin);
	}
	else
	{
		CLR_BIT(DDRC,pin);
	}
	break;
	case 'D':
	case 'd':
	if (dir == 1 )
	{
		SET_BIT(DDRD,pin);
	}
	else
	{
		CLR_BIT(DDRD,pin);
	}
	break;
	default:break;
}	
}
void DIO_WRITE_PIN(char port , char pin , char val)
{
	switch(port)
	{
		case 'A':
		case 'a':
		if (val == 1 )
		{
			SET_BIT(PORTA,pin);
		}
		else
		{
			CLR_BIT(PORTA,pin);
		}
		break;
		case 'B':
		case 'b':
		if (val == 1 )
		{
			SET_BIT(PORTB,pin);
		}
		else
		{
			CLR_BIT(PORTB,pin);
		}
		break;
		case 'C':
		case 'c':
		if (val == 1 )
		{
			SET_BIT(PORTC,pin);
		}
		else
		{
			CLR_BIT(PORTC,pin);
		}
		break;
		case 'D':
		case 'd':
		if (val == 1 )
		{
			SET_BIT(PORTD,pin);
		}
		else
		{
			CLR_BIT(PORTD,pin);
		}
		break;
}
}
void DIO_TOGGLE_PIN(char port , char pin )
{
	switch(port)
	{
		case 'A':
		case 'a':
		TOG_BIT(PORTA,pin);
		break;
		case 'B':
		case 'b':
		TOG_BIT(PORTB,pin);
		break;
		case 'C':
		case 'c':
		TOG_BIT(PORTC,pin);
		break;
		case 'D':
		case 'd':
		TOG_BIT(PORTD,pin);
		break;	
	}
}
unsigned char DIO_U8READ_PIN(char port , char pin )
{
	unsigned char value;
	switch(port)
	{
		case 'A':
		case 'a':
		value = READ_BIT(PINA,pin);
		break;
		case 'B':
		case 'b':
		value = READ_BIT(PINB,pin);
		break;
		case 'C':
		case 'c':
		value = READ_BIT(PINC,pin);
		break;
		case 'D':
		case 'd':
		value = READ_BIT(PIND,pin);
		break;
	}	
	return value ; 
}
void DIO_SET_PORT_DIR(char port , char dir)
{
	switch(port)
	{
		case 'A':
		case 'a':
		DDRA = dir;
		break;
		case 'B':
		case 'b':
		DDRB = dir;
		break;
		case 'C':
		case 'c':
		DDRC = dir;
		break;
		case 'D':
		case 'd':
		DDRD = dir;
		break;
		default:break;
	}
}
void DIO_WRITE_PORT(char port , char val)
{
	switch(port)
	{
		case 'A':
		case 'a':
		PORTA = val;
		break;
		case 'B':
		case 'b':
		PORTB = val;
		break;
		case 'C':
		case 'c':
		PORTC = val;
		break;
		case 'D':
		case 'd':
		PORTD = val;
		break;
		default:break;
	}
}
void DIO_TOGGLE_PORT(char port )
{
	switch(port)
	{
		case 'A':
		case 'a':
		PORTA = ~PORTA;
		break;
		case 'B':
		case 'b':
		PORTB = ~PORTB;
		break;
		case 'C':
		case 'c':
		PORTC = ~PORTC;
		break;
		case 'D':
		case 'd':
		PORTD = ~PORTD;
		break;
		default:break;
	}
}
unsigned char DIO_U8READ_PORT(char port )
{
	unsigned char valu ;
	switch(port)
	{
		case 'A':
		case 'a':
		valu = PINA;
		break;
		case 'B':
		case 'b':
		valu = PINB;
		break;
		case 'C':
		case 'c':
		valu = PINC;
		break;
		case 'D':
		case 'd':
		valu = PIND;
		break;
		default:break;
	}
	return valu;	
}
void DIO_venable_Pullup(char port , char pin , char status)
{
	switch (port)
	{	
	case 'A':
	case 'a':
	if (status == 1)
	{
		SET_BIT(PORTA,pin);
	}
	else
	{
		CLR_BIT(PORTA,pin);
	}
	break;
	case 'B':
	case 'b':
	if (status == 1)
	{
		SET_BIT(PORTB,pin);
	}
	else
	{
		CLR_BIT(PORTB,pin);
	}
	break;
	case 'C':
	case 'c':
	if (status == 1)
	{
		SET_BIT(PORTC,pin);
	}
	else
	{
		CLR_BIT(PORTC,pin);
	}
	break;
	case 'D':
	case 'd':
	if (status == 1)
	{
		SET_BIT(PORTD,pin);
	}
	else
	{
		CLR_BIT(PORTD,pin);
	}
	break;
	}
}

