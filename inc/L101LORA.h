#ifndef _L101_LORA_H_
#define _L101_LORA_H_


sbit L101_nRST   = P3^2;
sbit L101_nRELOAD   = P3^3;

#define L101RSTON	{L101_nRST = 0;}
#define L101RSTOFF	{L101_nRST = 1;}
#define L101RELOADON	{L101_nRELOAD = 0;}
#define L101RELOADOFF	{L101_nRELOAD = 1;}

typedef enum {
	L101_AT_OK = 0,
	L101_AT_E1,
	L101_AT_E2,
	L101_AT_E3,
	L101_AT_E4,
	L101_AT_E5
}L101ATSTA;


#define CR	0x0d
#define LF	0x0a

void L101_Reset(void);
void L101_Reload(void);



#endif /**/