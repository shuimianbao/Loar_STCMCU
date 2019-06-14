#include "type.h"

uint8_t Time0Cnt = 0;//用于定时器0内部计时用
//bit ComLostFlag = 0;//打开PA后,失去与上位机的通信标志

//extern uint8_t idata ucS2RecBufOutP;
//extern uint8_t idata ucS2RecBufInP;
/*
extern bit busy;
extern bit s1busy;
extern bit s1recflag;
extern bit s2recflag;
extern uint8_t idata s1rec;
extern uint8_t idata s2rec;
*/

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



void Delay10ms(void)		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 108;
	j = 144;
	do
	{
		while (--j);
	} while (--i);
}

void Delay100ms(void)		//@11.0592MHz
{
	uint8_t i, j, k;

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

void main(void)
{
	uint8_t i;//ItStatus1,ItStatus2;
	//uint8_t sum,sendcmd;
	uint16_t  counter;
	L101_PARA xL101Inst;
	L101ATSTA xRes;
	uint8_t ucSendTest[100],ucRecTest[100],ucTestlen;
	uint16_t usIdCrc;

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
/*	
	while(1)
	{
		if(s1recflag)//send to s2
		{
			if(busy==0)
			{
				S2BUF = s1rec;
				busy = 1;
				s1recflag = 0;
			}
		}

		if(s2recflag) //send to s1
		{
			if(s1busy==0)
			{
				SBUF = s2rec;
				s1busy = 1;
				s2recflag = 0;
			}
		}
	}
*/
	printf("Loar STC MCU\r\n");

	
	//printf("init: %bx,%bx\r\n",ucS2RecBufInP,ucS2RecBufOutP);

	xL101Inst.eMode = FP;			//定点模式
	xL101Inst.eRate = R21875BPS;	//定点模式下,速率要一致
	xL101Inst.ucCh = SELF_CH;				//模块1的信道设置为1
	xL101Inst.usAddr = SELF_ADDR;			//模块1的地址设置为1
	xL101Inst.ucPower = 20;			//发射功率为20db
	xL101Inst.eFec = OFF;			//FEC功能关闭

	xRes = InitL101Module(&xL101Inst);
	if (xRes != L101_AT_OK)
	{
		printf("Init Fail\r\n Reset MCU");
		while(1);
		//IAP_CONTR = 0x20;//reset from application code
	}
	printf("Init Success\r\n");
	usIdCrc = CalculatePacketCrc(xL101Inst.ulId);
	printf("ID:%x,crc:%x\r\n",xL101Inst.ulId,usIdCrc);
	
	counter=0;
	while(1)
	{
		//S2SendData("modue2 send\r\n",13);
		ucTestlen = sprintf(ucSendTest,"%s send test data \"abcdefghijklmnopqrstuvwxyz\" %u\r\n", MODULETYPE==1?"Master":"Salve", counter++ );
		
		L101_SendWithFPMode(ucSendTest,ucTestlen,DEST_CH,DEST_ADDR);
		for(i=0;i<10;i++)
			Delay100ms();//1S
		ucTestlen = L101_Receive(ucRecTest);
		if(ucTestlen)//had receive data
		{
			for(i=0;i<ucTestlen;i++)
			printf("%c",ucRecTest[i]);
		}
			
	}//end of while(1)
}

