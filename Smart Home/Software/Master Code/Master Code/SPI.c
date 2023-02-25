/*
 * SPI.c
 *
 * Created: 1/8/2023 11:53:04 PM
 *  Author: Ahmed Hassan
 */ 
#include "SPI.h"
void SPI_vInitMaster (void)
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
	//Set clock to fosc/16
	SET_BIT(SPCR,SPR0);
}

unsigned char SPI_MasterTransmit_Data(unsigned char Data)
{
	
	//Clear SS to send data to slave/
	DIO_WRITE_PIN('B',4,0);
	//Put data on SPDR*/
	SPDR=Data;
	//read SPDR*/
	while (((SPSR&(1<<SPIF))>>SPIF)==0);//wait till the transmitting is done
	return SPDR ;//data which receive from slave
	
	
	
	/*
	SPDR = Data;//move the given data to SPI Data register
	
	//in case of master send data and wait till the data are fully transmitted
	//in case of slave wait the data to be transmitted by the master
	while (((SPSR&(1<<SPIF))>>SPIF)==0)	//wait till the transmitting is done
	;									//SPI flag is set to one if the transmission is done
	
	return SPDR;//return the value of SPI Data register after the transmission or receiving is done
	*/
	
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