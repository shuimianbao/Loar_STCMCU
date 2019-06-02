#define _L101_LORA_H_
#define _L101_LORA_H_


sbit L101_nRST   = P3^2;
sbit L101_nRELOAD   = P3^3;

#define L101RSTON	{L101_nRST = 0;}
#define L101RSTOFF	{L101_nRST = 1;}
#define L101RELOADON	{L101_nRELOAD = 0;}
#define L101RELOADOFF	{L101_nRELOAD = 1;}

void L101_Reset(void);
void L101_Reload(void);



#endif /**/