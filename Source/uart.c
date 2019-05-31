#include "type.h"

bit UartSendFlag = 0;
bit UartRecFlag = 0;
bit UPakageRec = 0;
bit startflag = 0;
unsigned char tmpUart;
unsigned char buflen;
unsigned char xdata buf[256]; //
unsigned char RecState = STATE_START_A;
void Init_Uart(void)
{
		// 7   6   5   4   3   2   1  0<input type="password" >
		//SM0 SM1 SM2 REN TB8 RB8 TI RI
		// 0   1   0   1   0   0   0  0
		SCON = 0X50;								//set UART mode as 8-bit variable baudrate, enable receive
		//SCON = 0x5a;                //set UART mode as 8-bit variable baudrate
    TMOD |= 0x20;                //timer1 as 8-bit auto reload mode
    AUXR |= 0x40;                //timer1 work at 1T mode
    TH1 = TL1 = BAUD;           //9600 bps
    TR1 = 1;
    ES =1;										  //enable UART0 interrupt
		//TI = 1;
}

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