#ifndef _TYPE_H_
#define _TYPE_H_
#include <intrins.h>
#include <stdio.h>
#include <string.h>
#include "gloabdef.h"
#include "stc12c5a60s2.h"
#include "spi.h"
#include "uart.h"
#include "eeprom.h"
#include "l101lora.h"
#include "crc.h"
#include "lr30lora.h"


	
#define FOSC			11059200L
#define BAUD(X)		  (256 - FOSC/32/X)
#define BAUD9600	  (256 - FOSC/32/9600)

sbit SPI_MOSI = P1^5;
sbit SPI_MISO = P1^6;
sbit SPI_CLK  = P1^7;

sbit LED1     = P4^5;
sbit LED2     = P4^4;

#define LED1ON    {LED1 = 0;}
#define LED1OFF   {LED1 = 1;}
#define LED2ON    {LED2 = 0;}
#define LED2OFF   {LED2 = 1;}


//Internal extend 1KBytes RAM enable bit of AUXR
#define EXTRAM 0X02

#define PAWDTIME (uint8_t)(10000/50)   //¶¨Ê±10S

void Delay10ms(void);
void Delay100ms(void);

void LR30_Reset(void);


//macro for test
#define MASTERNODE		1
#define SALVENODE		2
#define MODULETYPE		MASTERNODE

#if MODULETYPE == MASTERNODE
	#define SELF_CH		1
	#define	SELF_ADDR	1
	#define DEST_CH		2
	#define DEST_ADDR	2
#else
	#define SELF_CH		2
	#define	SELF_ADDR	2
	#define DEST_CH		1
	#define DEST_ADDR	1
#endif


#endif /*_TYPE_H_*/
