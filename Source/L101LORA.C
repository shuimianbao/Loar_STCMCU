#include "type.h"

void L101_Reset(void)
{
	uint8_t i;
	L101RSTON;
	for(i=0;i<10;i++)
		Delay100ms();//1S reset
	L101RSTOFF;
}

void L101_Reload(void)
{
	uint8_t i;
	L101RELOADON;
	for(i=0;i<50;i++)
		Delay100ms();//5S reset
	L101RELOADOFF;
}

L101ATSTA L101_EnterATMode(void)
{
	S2SendData("+++",3);
	//while()
}
