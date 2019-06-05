#ifndef _LR30LORA_H_
#define _LR30LORA_H_

sbit LR30_SS  = P1^4;
sbit LR30_nRST   = P4^3;
sbit LR30_DIO0	 = P4^0;
sbit LR30_DIO1	 = P3^1;
sbit LR30_DIO2	 = P3^6;
sbit LR30_DIO3	 = P1^0;
sbit LR30_DIO4	 = P1^1;
sbit LR30_DIO5	 = P4^2;


#define LR30RSTON	{LR30_nRST = 0;}
#define LR30RSTOFF	{LR30_nRST = 1;}

void LR30_Reset(void);

#endif /*_LR30LORA_H_*/
