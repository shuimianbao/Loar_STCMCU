#include "type.h"

bit busy = 0;
/*
bit s1busy = 0;
bit s1recflag = 0;
bit s2recflag = 0;
uint8_t idata s1rec;
uint8_t idata s2rec;
*/
bit bS2BufOverflow = 0;
uint8_t xdata ucS2RecBuf[S2RECBUFLEN];
uint8_t idata ucS2RecBufInP;
uint8_t idata ucS2RecBufOutP;
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
	//ES =1;										  //enable UART0 interrupt
}

/*
void SendChar(uint8_t c)
{
	
	//while (!TI);
  //	TI = 0;
	SBUF =c;
	while(0 == UartSendFlag);
	UartSendFlag = 0;
	
}


void uart_ISR(void) interrupt 4
{
	//uint8_t idata tmpUart;
	
	if(TI)
	{
		//UartSendFlag =1;
		s1busy = 0;
		TI =0;
	}	
	
	if(RI)
	{
		s1rec = SBUF;
		s1recflag =1;	
		RI = 0;
	}
}
*/

void Init_Uart2(void)
{
	ucS2RecBufOutP = ucS2RecBufInP = 0;
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
void S2SendByte(uint8_t dat)
{
    while (busy);           //Wait for the completion of the previous data is sent
    busy = 1;
    S2BUF = dat;            //Send data to UART2 buffer
}


void S2SendData(uint8_t *buf, uint8_t len)
{
	uint8_t i;
	printf("send:");
	for(i=0;i<len;i++)
	{
		S2SendByte(buf[i]);
		printf("%c",buf[i]);
	}
	printf("\r\n");
}

/*
uint8_t S2ReadData(uint8_t xdata *buf,uint8_t delay)
{
	uint8_t x,len=0;
	while(delay--)
		Delay10ms();
	printf("bufs 0x%x",buf);
	printf(" 0x%x\r\n",&buf);
	
	printf("read:");
	if(bS2BufOverflow) //over flow
	{
		//Todo:...
		bS2BufOverflow = 0;
		printf("S2 Buffer overlfow\r\n");
	}
	len = (uint8_t)(((uint16_t)ucS2RecBufInP + 256 - (uint16_t)ucS2RecBufOutP) % 256);
	buf = ucS2RecBuf + ucS2RecBufOutP;
	ucS2RecBufOutP += len;
	printf("len: %bu,",ucS2RecBufInP);
	printf("%bu,",ucS2RecBufOutP);
	printf("%bu\r\n",len);
	
	printf("buf 0x%x",ucS2RecBuf);
	printf(" 0x%x\r\n",&ucS2RecBuf);
	printf(" 0x%x",buf);
	printf(" 0x%x\r\n",&buf);
	if(len)
	{
		for(x=0;x<len;x++)
			printf("%c",buf[x]);
	}
	
	return len;
	
}
*/
uint8_t* S2ReadData(uint8_t     *buflen,uint8_t delay)
{
	uint8_t x;
	uint8_t *buf;
	*buflen = 0;
	while(delay--)
		Delay10ms();
	printf("bufs 0x%x",buf);
	printf(" 0x%x\r\n",&buf);
		
	printf("read:");
	if(bS2BufOverflow) //over flow
	{
		//Todo:...
		bS2BufOverflow = 0;
		printf("S2 Buffer overlfow\r\n");
	}
	*buflen = (uint8_t)(((uint16_t)ucS2RecBufInP + 256 - (uint16_t)ucS2RecBufOutP) % 256);
	buf = ucS2RecBuf + ucS2RecBufOutP;
	ucS2RecBufOutP += *buflen;
	printf("len: %bu,",ucS2RecBufInP);
	printf("%bu,",ucS2RecBufOutP);
	printf("%bu\r\n",*buflen);
		
	printf("buf 0x%x",ucS2RecBuf);
	printf(" 0x%x\r\n",&ucS2RecBuf);
	printf(" 0x%x",buf);
	printf(" 0x%x\r\n",&buf);
	if(*buflen)
	{
		for(x=0;x<*buflen;x++)
			printf("%c",buf[x]);
	}
		
	return buf;
		
}

/*----------------------------
UART2 interrupt service routine
----------------------------*/

void Uart2(void) interrupt 8
{
	//uint8_t temp;
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;     //Clear receive interrupt flag
        ucS2RecBuf[ucS2RecBufInP] = S2BUF;         //put UART data in buf
        ucS2RecBufInP++;
		
		if(ucS2RecBufInP == ucS2RecBufOutP)
			bS2BufOverflow = 1;	//buf over flow
				
		//printf("%c",temp);
		LED1 =~LED1;
    }
	
    if (S2CON & S2TI)
    {
        S2CON &= ~S2TI;     //Clear transmit interrupt flag
        busy = 0;           //Clear transmit busy flag
		LED2 =~LED2;
    }
}
/*
	void Uart2(void) interrupt 8
	{
		uint8_t idata temp;
		if (S2CON & S2RI)
		{
			S2CON &= ~S2RI; 	//Clear receive interrupt flag
			
			s2rec = S2BUF;
			s2recflag = 1;
			LED1 =~LED1;
		}
		
		if (S2CON & S2TI)
		{
			S2CON &= ~S2TI; 	//Clear transmit interrupt flag
			busy = 0;			//Clear transmit busy flag
			LED2 =~LED2;
		}
	}
*/
