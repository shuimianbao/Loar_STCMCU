#include "type.h"

void L101_Reset(void)
{
	unsigned char i;
	L101RSTON;
	for(i=0;i<10;i++)
		Delay100ms();//1S reset
	L101RSTOFF;
}
void L101_Reload(void)
{
	unsigned char i;
	L101RELOADON;
	for(i=0;i<50;i++)
		Delay100ms();//5S reset
	L101RELOADOFF;
}