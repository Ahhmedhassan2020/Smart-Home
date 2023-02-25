/*
 * ADC.c
 *
 * Created: 12/28/2022 10:18:06 AM
 *  Author: Ahmed Hassan
 */ 
#include <avr/io.h>
#include "std_macros.h"
void ADC_vinit(void)
{
	/*configure VREF to the internal voltage(2.56 V)*/
	SET_BIT(ADMUX,REFS0);
	SET_BIT(ADMUX,REFS1);
	/*************************************/
	SET_BIT(ADCSRA,ADEN); // ENable ADC
	
	/* adjust ADC clock by select the division factor----->64*/
	SET_BIT(ADCSRA,ADPS2);
	SET_BIT(ADCSRA,ADPS1);
	/***********************************/
}
unsigned short ADC_u16Read(void)
{
	unsigned char read_val ; 
	SET_BIT(ADCSRA,ADSC);//start conversion 
		while(READ_BIT(ADCSRA,ADSC)==1);//stay in your position till ADSC become 0	read_val=(ADCL);	read_val|=(ADCH<<8);	return read_val ;
	
}