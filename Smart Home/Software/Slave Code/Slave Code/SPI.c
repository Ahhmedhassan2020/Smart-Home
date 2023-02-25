/*
 * SPI.c
 *
 * Created: 1/8/2023 11:53:04 PM
 *  Author: Ahmed Hassan
 */ 
#include "SPI.h"
void SPI_MasterInit(void)
{
	/*Set SS / MOSI / SCK  as output pins for master*/
	DIO_SET_PIN_DIR('B',4,1);//SS
	DIO_SET_PIN_DIR('B',5,1);//MOSI
	DIO_SET_PIN_DIR('B',7,1);//SCK
	/*set SS to high */
	DIO_WRITE_PIN('B',4,1);
	/*Enable SPI*/
	SET_BIT(SPCR,SPE);
	/*Enable Master mode*/
	SET_BIT(SPCR,MSTR);
	/*Enable interrupt*/
	SET_BIT(SPCR,SPIE);
	/*Set clock to fosc/16*/
	SET_BIT(SPCR,SPR0);
}
void SPI_SlaveInit(void)
{
	/*Enable SPI*/
	SET_BIT(SPCR,SPE);
	/*Set MISO as output*/
	DIO_SET_PIN_DIR('B',6,1);
}
unsigned char SPI_MasterTransmit_Data(unsigned char Data)
{
	/*Clear SS to send data to slave*/
	DIO_WRITE_PIN('B',4,0);
	/*Put data on SPDR*/
	SPDR=Data;
	/*read SPDR*/
	return SPDR ;//data which receive from slave  
	
}

unsigned char SPI_SlaveReceive_Data(unsigned char Data)
{
	/*Put data on SPDR*/
	SPDR=Data;
	/*Wait until data is received in SPI register*/
	while(READ_BIT(SPSR,SPIF)==0);
	/*read the received data*/
	return SPDR ;//data which receive from master 
}
void SPI_MasterTransmitstring(unsigned char *ptr)
{
	while((*ptr)!=0)
	{
		SPI_MasterTransmit_Data(*ptr);
		_delay_ms(100);
		ptr++;
	}
}