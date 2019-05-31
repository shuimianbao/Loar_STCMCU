#ifndef _UART_H_
#define _UART_H_

#define USARTFLAG	'*'
#define UENDFLAG	'#'

#define PKG_START_FLAG	0x1E //帧起始标识符
#define PKG_END_FLAG	0x7F //帧结束标示符
#define PKG_DLE_FLAG	0x1F //特殊控制符

#define STATE_START_A		0 //等待第一个起始标识符状态
#define STATE_START_B		1 //等待第二个起始标识符状态
#define STATE_VAILD_DATA	2 //等待有效数据状态
#define STATE_END			3 //等待第二个结束标示符状态
#define STATE_DLE			4 //接收到特殊控制符状态


//extern bit UartSendFlag;
extern bit UartRecFlag;
extern bit UPakageRec;
extern unsigned char tmpUart;
extern unsigned char buflen;
extern unsigned char xdata buf[256]; //
void Init_Uart(void);
void SendChar(unsigned char c);
void RecUartPak(void);


#endif /*_UART_H_*/