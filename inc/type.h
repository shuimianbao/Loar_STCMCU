#ifndef _TYPE_H_
#define _TYPE_H_
#include <intrins.h>
#include <stdio.h>
#include "stc12c5a60s2.h"
#include "spi.h"
#include "uart.h"
#include "eeprom.h"
#include "l101lora.h"
#include "gloabdef.h"

	
#define FOSC			11059200L
#define BAUD(X)		  (256 - FOSC/32/X)
#define BAUD9600	  (256 - FOSC/32/9600)
sbit SPI_MOSI = P1^5;
sbit SPI_MISO = P1^6;
sbit SPI_CLK  = P1^7;
sbit LR30_SS  = P1^4;

sbit LR30_nRST   = P4^3;
sbit LR30_DIO0	 = P4^0;
sbit LR30_DIO1	 = P3^1;
sbit LR30_DIO2	 = P3^6;
sbit LR30_DIO3	 = P1^0;
sbit LR30_DIO4	 = P1^1;
sbit LR30_DIO5	 = P4^2;



sbit LED1     = P4^5;
sbit LED2     = P4^4;


#define LED1ON    {LED1 = 0;}
#define LED1OFF   {LED1 = 1;}
#define LED2ON    {LED2 = 0;}
#define LED2OFF   {LED2 = 1;}



#define LR30RSTON	{LR30_nRST = 0;}
#define LR30RSTOFF	{LR30_nRST = 1;}

//Internal extend 1KBytes RAM enable bit of AUXR
#define EXTRAM 0X02

#define PAWDTIME (uint8_t)(10000/50)   //¶¨Ê±10S

void Delay100ms(void);

void LR30_Reset(void);

#endif /*_TYPE_H_*/
