#ifndef _TYPE_H_
#define _TYPE_H_
#include <intrins.h>
#include "stc12c5a60s2.h"
#include "spi.h"
#include "uart.h"
#include "eeprom.h"

#define FOSC			11059200L
#define BAUD		  (256 - FOSC/32/9600)
sbit SPI_MOSI = P1^5;
sbit SPI_MISO = P1^6;
sbit SPI_CLK  = P1^7;

sbit RF_INT   = P3^2;
sbit RF_SDN   = P3^7;
sbit RF_SEL   = P1^4;



sbit IO_PAC		= P0^0;
//sbit RF_INT   = P3^3;  //用在模块上IO_INT
//sbit RF_SEL   = P0^1;  //用在模块上IO_SEL
//sbit RF_SDN   = P0^2;		//用在模块上IO_SDN

sbit TEMP   = P0^3;
sbit FAN_CTL   = P0^4;
sbit PA_CTL   = P0^5;
sbit LED1     = P0^6;
sbit LED2     = P0^7;



//sbit SHDN_LNA = P3^6;

sbit KEY1     = P4^3;
sbit KEY2     = P4^2;


#define LED1ON    {LED1 = 0;}
#define LED1OFF   {LED1 = 1;}
#define LED2ON    {LED2 = 0;}
#define LED2OFF   {LED2 = 1;}

//Internal extend 1KBytes RAM enable bit of AUXR
#define EXTRAM 0X02

#define PAWDTIME (unsigned char)(10000/50)   //定时10S

#endif /*_TYPE_H_*/
