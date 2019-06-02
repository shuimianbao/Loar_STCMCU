#include "type.h"

unsigned char Time0Cnt = 0;//用于定时器0内部计时用
//bit ComLostFlag = 0;//打开PA后,失去与上位机的通信标志
void InitMcuPort(void)
{
// P1.7     P1.6     P1.5     P1.4     P1.3     P1.2     P1.1     P1.0
//SPICLK    MISO     MOSI    RF_SEL   TxD2      RxD2  LR30_IO4  LR30_IO3
//P1M0.7   P1M0.6   P1M0.5   P1M0.4   P1M0.3   P1M0.0   P1M0.1   P1M0.0 
//  1        0        1        1        1        0        0        0
//P1M1.7   P1M1.6   P1M1.5   P1M1.4   P1M1.3   P1M1.0   P1M1.1   P1M1.0
//  0        1        0        0        0        1        0        0
//S_Pull   Input    S_Pull   S_Pull
	P1M0 = 0XB8;
	P1M1 = 0X44;

// P0.7     P0.6     P0.5     P0.4     P0.3     P0.2     P0.1     P0.0
//   
//P0M0.7   P0M0.6   P0M0.5   P0M0.4   P0M0.3   P0M0.2   P0M0.1   P0M0.0 
//  1        1        1        1        0        0        0        0
//P0M1.7   P0M1.6   P0M1.5   P0M1.4   P0M1.3   P0M1.2   P0M1.1   P0M1.0
//  0        0        0        0        0        0        0        0
//S_Pull   S_Pull   S_Pull   S_Pull   Normal   Normal   Normal 
//	P0M0 = 0XF0;
//	P0M1 = 0X00;

// P3.7     P3.6     P3.5     P3.4     P3.3     P3.2     P3.1     P3.0
//LR30_IO1 LR30_IO2                 L101_Reload  L101_RST  UART_TX  UART_RX  
//P3M0.7   P3M0.6   P3M0.5   P3M0.4   P3M0.3   P3M0.2   P3M0.1   P3M0.0 
//  0        0        0        0        1        1        1        0
//P3M1.7   P3M1.6   P3M1.5   P3M1.4   P3M1.3   P3M1.2   P3M1.1   P3M1.0
//  0        0        0        0        0        0        0        1
//  0   	 0                        S_Pull   S_Pull    S_Pull   Input
	P3M0 = 0X0E;
	P3M1 = 0X01;
	
// P4.7     P4.6     P4.5     P4.4     P4.3     P4.2     P4.1     P4.0
//                   LED1     LED2   LR30_RST LR30_IO5           LR30_IO0
//P4M0.7   P4M0.6   P4M0.5   P4M0.4   P4M0.3   P4M0.2   P4M0.1   P4M0.0 
//  0        0        1        1       1        0        0        0
//P4M1.7   P4M1.6   P4M1.5   P4M1.4   P4M1.3   P4M1.2   P4M1.1   P4M1.0
//  0        0        0        0       0        0        0        0
//                  S_Pull   S_Pull   S_Pull         
	P4M0 = 0x38;
	P4M1 = 0x00;
	P4SW = 0xe0;

	

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
			;//ComLostFlag = 1;
	}
	TH0 = 0x4C;//定时50ms
	TL0 = 0x00;
}



void Delay100ms(void)		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void LR30_Reset(void)
{
	unsigned char i;
	LR30RSTON;
	for(i=0;i<10;i++)
		Delay100ms();//1S reset
	LR30RSTOFF;
}
void main(void)
{
	unsigned char i;//ItStatus1,ItStatus2;
	//unsigned char sum,sendcmd;
	unsigned int  counter;

	CLK_DIV = 0;//fsys= fxtal
	InitMcuPort();

	Init_Uart();
	Init_Uart2();
	Init_SPI();
	Init_Time0();

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
	
	L101_Reset();
	printf("Loar STC MCU\r\n");
	
	while(1)
	{
		S2SendData("modue2 send\r\n",13);
		for(i=0;i<10;i++)
			Delay100ms();//1S
			
	}//end of while(1)
}

