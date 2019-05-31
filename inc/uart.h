#ifndef _UART_H_
#define _UART_H_

#define USARTFLAG	'*'
#define UENDFLAG	'#'

#define PKG_START_FLAG	0x1E //֡��ʼ��ʶ��
#define PKG_END_FLAG	0x7F //֡������ʾ��
#define PKG_DLE_FLAG	0x1F //������Ʒ�

#define STATE_START_A		0 //�ȴ���һ����ʼ��ʶ��״̬
#define STATE_START_B		1 //�ȴ��ڶ�����ʼ��ʶ��״̬
#define STATE_VAILD_DATA	2 //�ȴ���Ч����״̬
#define STATE_END			3 //�ȴ��ڶ���������ʾ��״̬
#define STATE_DLE			4 //���յ�������Ʒ�״̬


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