#include "type.h"

#if 0

void Init_SPI(void)
{
	SPDAT = 0;                                 //initial SPI data
    SPSTAT = SPIF | WCOL;                     //clear SPI status
    //SPCTL = SSIG | SPEN | MSTR | CPHA;        //master mode
	SPCTL = SSIG | SPEN | MSTR ;        //master mode
}

void SPI_WriteByte( uint8_t input )
{
	  SPDAT = input;                //trigger SPI send
    while (!(SPSTAT & SPIF));   //wait send complete
    SPSTAT = SPIF | WCOL;       //clear SPI status

  
}

uint8_t SPI_ReadByte(void)
{
		SPDAT = 0xFF;                //trigger SPI send
    while (!(SPSTAT & SPIF));   //wait send complete
    SPSTAT = SPIF | WCOL;       //clear SPI status
    return SPDAT;
}

#else

void Delay(uint8_t i) { while(--i); }

void Init_SPI(void)
{
	SPI_CLK=0;
}

void SPI_WriteByte( uint8_t input )
{
	uint8_t i=0; 
	for (i=0;i<=7;i++) 
	{
	 	if (input & 0x80) 
	      SPI_MOSI = 1;
	    else 
	      SPI_MOSI = 0;
	    SPI_CLK = 1;                             // clock data in
	    input <<= 1;
	    Delay(1);
	    SPI_CLK = 0;
	    Delay(1);
	}  

  
}

uint8_t SPI_ReadByte(void)
{
	uint8_t output=0x00;   
  	uint8_t i=0; 
  	bit temp;
  	Delay(10);
  	for(i=0;i<=7;i++)    
  	{
    	output = output<<1;
		SPI_CLK =1;
    	Delay(1);
		temp = SPI_MISO;
    	SPI_CLK =0;                   
    	Delay(1);
    	//temp = SPI_MISO;
    	if (temp)
      	output |= 0x01; 
    
              
  	}       
  	return output;  
}

#endif
