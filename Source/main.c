#include "type.h"
#include <stdio.h>

unsigned char Time0Cnt = 0;//用于定时器0内部计时用
bit ComLostFlag = 0;//打开PA后,失去与上位机的通信标志
void InitMcuPort(void)
{
// P1.7     P1.6     P1.5     P1.4     P1.3     P1.0     P1.1     P1.0
//SPICLK    MISO     MOSI    RF_SEL
//P1M0.7   P1M0.6   P1M0.5   P1M0.4   P1M0.3   P1M0.0   P1M0.1   P1M0.0 
//  1        0        1        1        0        0        0        0
//P1M1.7   P1M1.6   P1M1.5   P1M1.4   P1M1.3   P1M1.0   P1M1.1   P1M1.0
//  0        1        0        0        0        0        0        0
//S_Pull   Input    S_Pull   S_Pull
	P1M0 = 0XB0;
	P1M1 = 0X40;

// P0.7     P0.6     P0.5     P0.4     P0.3     P0.2     P0.1     P0.0
// LED2     LED1    IO_SDN   IO_PAC   IO_IO2   IO_IO1   IO_IO0  
//P0M0.7   P0M0.6   P0M0.5   P0M0.4   P0M0.3   P0M0.2   P0M0.1   P0M0.0 
//  1        1        1        1        0        0        0        0
//P0M1.7   P0M1.6   P0M1.5   P0M1.4   P0M1.3   P0M1.2   P0M1.1   P0M1.0
//  0        0        0        0        0        0        0        0
//S_Pull   S_Pull   S_Pull   S_Pull   Normal   Normal   Normal 
	P0M0 = 0XF0;
	P0M1 = 0X00;

// P3.7     P3.6     P3.5     P3.4     P3.3     P3.2     P3.1     P3.0
//RF_SDN  SHDN_LNA                   I0_INT1  RF_INT0  UART_TX  UART_RX  
//P3M0.7   P3M0.6   P3M0.5   P3M0.4   P3M0.3   P3M0.2   P3M0.1   P3M0.0 
//  1        1        0        0        0        0        1        0
//P3M1.7   P3M1.6   P3M1.5   P3M1.4   P3M1.3   P3M1.2   P3M1.1   P3M1.0
//  0        0        0        0        1        1        0        1
//S_Pull   S_Pull                     Input    Inupt    S_Pull   Input
	P3M0 = 0Xc2;
	P3M1 = 0X0d;
	
// P4.7     P4.6     P4.5     P4.4     P4.3     P4.2     P4.1     P4.0
//                                  Key1/TXD2 Key2/RXD2        IO_SPI_SEL
//P4M0.7   P4M0.6   P4M0.5   P4M0.4   P4M0.3   P4M0.2   P4M0.1   P4M0.0 
//  0        0        0        0       0/1        0        0        1
//P4M1.7   P4M1.6   P4M1.5   P4M1.4   P4M1.3   P4M1.2   P4M1.1   P4M1.0
//  0        0        0        0       1/0        1        0        0
//                                 Input/S_Pull Inupt             S_Pull
	P4M0 = 0x09;//0X01;
	P4M1 = 0x04;//0X0C;

	PA_CTL = 0;
	FAN_CTL = 0;
	IO_PAC = 1;

}
void Init_Uart2(void)
{
	S2CON = 0X50;		//set UART2 mode as 8-bit variable baudrate, enable receive
	BRT = BAUD;
	AUXR1 |= 0X10;
	AUXR &= 0xE3;
	AUXR |= 0x14;
	
	
}
void S2SendChar(unsigned char c)
{
	
	//while (!TI);
  //	TI = 0;
	S2BUF =c;
	while(0 == (S2CON & 0x02));
	S2CON &= ~0x02;
	
}

//定时器1用于通信看门狗,
//如果PA打开后在10S钟内上位机没有发送数据
//则关闭PA
void Init_Time0(void)
{
	TMOD |= 0x01;//16bits timer,不分频
	TH0 = 0x4C;//定时50ms
	TL0 = 0x00;
	ET0 = 1;//定时器0允许中断
}
void Time0_ISR(void) interrupt 1
{
	if(Time0Cnt)
	{
		Time0Cnt --;
		if(0==Time0Cnt)
			ComLostFlag = 1;
	}
	TH0 = 0x4C;//定时50ms
	TL0 = 0x00;
}

void OpenPA(void)
{
	LED1ON;//for test

	PA_CTL = 1;
	FAN_CTL = 1;
					
	TH0 = 0x4C;//定时50ms
	TL0 = 0x00;
	Time0Cnt = PAWDTIME;//打开定时器0
	ComLostFlag = 0;
	TR0 = 1;
}

void ClosePA(void)
{
	LED1OFF;//For test
	//关闭功放
	FAN_CTL = 0;
	PA_CTL = 0;
	TR0 = 0;//关闭定时器0
	ComLostFlag = 0;
}
void main(void)
{
	unsigned char i;//ItStatus1,ItStatus2;
	unsigned char sum,sendcmd;
	unsigned int  counter;
	//unsigned char aa[10] = {'1','2','3','4','5','6','7','8','9','0'};
	//unsigned char bb[10] ={0};
	InitMcuPort();
	RF_SDN = 0;
	Init_Uart();
	Init_Uart2();
	Init_SPI();
	Init_Time0();

	//printf("Radio Start....\n");
	//SI4432_WriteReg(0x07, 0x80);					//write 0x80 to the Operating & Function Control1 register 
	AUXR &= ~EXTRAM; //Clear EXTRAM, enable access intenal 1KBytes RAM
	EA =1;					 //Open Interrupts
	LED1ON;
	for(counter=0;counter<0xffff;counter++);
	LED2ON;
	for(counter=0;counter<0xffff;counter++);
	LED1OFF;
	for(counter=0;counter<0xffff;counter++);
	LED2OFF;
	for(counter=0;counter<0xffff;counter++);


	while(1)
	{

			
	}//end of while(1)
}
