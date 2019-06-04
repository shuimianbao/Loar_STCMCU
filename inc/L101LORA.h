#ifndef _L101_LORA_H_
#define _L101_LORA_H_


sbit L101_nRST   = P3^2;
sbit L101_nRELOAD   = P3^3;

#define L101RSTON	{L101_nRST = 0;}
#define L101RSTOFF	{L101_nRST = 1;}
#define L101RELOADON	{L101_nRELOAD = 0;}
#define L101RELOADOFF	{L101_nRELOAD = 1;}

#define MAXTRY 3
#define AT_READ_DELAY 1 //10ms
typedef enum {
	L101_AT_OK = 0,	//响应成功
	L101_AT_E1,		//无效的命令格�?
	L101_AT_E2,		//无效的命�?
	L101_AT_E3,		//无效的操作符
	L101_AT_E4,		//无效的参�?
	L101_AT_E5,		//操作不允�?
	L101_AT_E6		//other
}L101ATSTA;

typedef enum {
	OK =0,
	FAIL
}STATUS;
typedef enum {
	ON =0,
	OFF
}ENABLE;
typedef enum{
	TRANS = 0,	//透传模式  （默认状态）
	FP			//定点模式
}L101_WROKMODE;

typedef enum {
	R268BPS = 1,
	R488BPS,
	R537BPS,
	R878BPS,
	R977BPS,
	R1758BPS,
	R3125BPS,
	R6250BPS,
	R10937BPS,
	R21875BPS
}L101_RATE;


typedef struct {
	L101_WROKMODE eMode;//工作模式,	TRANS:透传模式  (默认状�? FP:定点模式
	L101_RATE eRate;	//空中速率等级1~10(默认10)
	uint16_t usAddr;	//目标地址, 0~65535(默认0),65535 为广播地址,同信道同速率的模块都能接�?
	uint8_t ucCh;		//信道0~127(默认72信道,对应470Mhz)
	ENABLE eFec;		//前向纠错
	uint8_t ucPower;	//发射功率10~20db(默认20db)
	uint32_t ulId;		//节点 ID,固化在模块中，只�?
}L101_PARA;

#define CR	0x0d
#define LF	0x0a

void L101_Reset(void);
void L101_Reload(void);
L101ATSTA L101_ATResoponSearch(const uint8_t *pucStr, uint8_t *pucBuf, uint8_t ucBuflen);
L101ATSTA L101_EnterATMode(void);
L101ATSTA L101_ExitATMode(void);
L101ATSTA L101_ReadModuleId(uint32_t *ulId);
L101ATSTA L101_ReadModuleVer(void);
L101ATSTA L101_SetWorkMode(L101_WROKMODE eMode);
L101ATSTA L101_SetRate(L101_RATE eRate);
L101ATSTA L101_SetAddress(uint16_t usAddr);
L101ATSTA L101_SetChannle(uint8_t ucCh);
L101ATSTA L101_SetFec(ENABLE eEn);
L101ATSTA L101_SetTxPower(uint8_t ucPa);
L101ATSTA InitL101Module(L101_PARA * xInstPara);


#endif /**/
