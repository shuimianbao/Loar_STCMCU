#include "type.h"


void LR30_Reset(void)
{
	uint8_t i;
	LR30RSTON;
	for(i=0;i<10;i++)
		Delay100ms();//1S reset
	LR30RSTOFF;
}


