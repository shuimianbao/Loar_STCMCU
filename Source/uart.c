#include "type.h"

bit busy;

void Init_Uart(void)
{
		// 7   6   5   4   3   2   1  0<input type="password" >
		//SM0 SM1 SM2 REN TB8 RB8 TI RI
		// 0   1   0   1   0   0   0  0
		SCON = 0X50;								//set UART mode as 8-bit variable baudrate, enable receive
		//SCON = 0x5a;                //set UART mode as 8-bit variable baudrate
    TMOD |= 0x20;                //timer1 as 8-bit auto reload mode
    AUXR |= 0x40;                //timer1 work at 1T mode
    TH1 = TL1 = BAUD(115200);           //9600 bps
    TR1 = 1;
    ES =0;										  //enable UART0 interrupt
	TI = 1;
}
/*
void SendChar(unsigned char c)
{
	
	//while (!TI);
  //	TI = 0;
	SBUF =c;
	while(0 == UartSendFlag);
	UartSendFlag = 0;
	
}


void uart_ISR(void) interrupt 4
{
	
	
	if(TI)
	{
		UartSendFlag =1;
		TI =0;
	}	
	
	if(RI)
	{
		tmpUart = SBUF;
		UartRecFlag =1;	
		RI = 0;
	}
}
*/
void Init_Uart2(void)
{
	S2CON = 0X50;		//set UART2 mode as 8-bit variable baudrate, enable receive
	BRT = BAUD(115200);
	//AUXR1 |= 0X10;
	AUXR &= 0xE3;
	AUXR |= 0x14;
	IE2 |= 0x01;             //Enable UART2 interrupt
	
	
}
/*----------------------------
Send a byte data to UART
Input: dat (data to be sent)
Output:None
----------------------------*/
void S2SendByte(unsigned char dat)
{
    while (busy);           //Wait for the completion of the previous data is sent
    busy = 1;
    S2BUF = dat;            //Send data to UART2 buffer
}


void S2SendData(unsigned char *buf, unsigned char len)
{
	unsigned char i;
	for(i=0;i<len;i++)
		S2SendByte(buf[i]);
}
/*----------------------------
UART2 interrupt service routine
----------------------------*/
void Uart2(void) interrupt 8
{
	unsigned char temp;
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;     //Clear receive interrupt flag
        temp = S2BUF;         //P0 show UART data
		printf("%c",temp);
		LED1 =~LED1;
    }
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;     //Clear transmit interrupt flag
        busy = 0;           //Clear transmit busy flag
		LED2 =~LED2;
    }
}
