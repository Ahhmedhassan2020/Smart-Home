/*
 * SPI.h
 *
 * Created: 1/8/2023 11:52:21 PM
 *  Author: Ahmed Hassan
 */ 


#ifndef SPI_H_
#define SPI_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include "DIO.h"
#include "std_macros.h"
#include "STD_MESSAGES.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#define SPI_SCK  7
#define SPI_MISO 6
#define SPI_MOSI 5
#define SPI_SS   4
void SPI_MasterInit(void);
void SPI_SlaveInit(void);
unsigned char SPI_MasterTransmit_Data(unsigned char Data);
unsigned char SPI_SlaveReceive_Data(unsigned char Data);
void SPI_MasterTransmitstring(unsigned char *ptr);
#endif /* SPI_H_ */