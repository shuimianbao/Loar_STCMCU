#ifndef _SPI_H_
#define _SPI_H_

#define SPIF        0x80        //SPSTAT.7
#define WCOL        0x40        //SPSTAT.6

#define SSIG        0x80        //SPCTL.7
#define SPEN        0x40        //SPCTL.6
#define DORD        0x20        //SPCTL.5
#define MSTR        0x10        //SPCTL.4
#define CPOL        0x08        //SPCTL.3
#define CPHA        0x04        //SPCTL.2
#define SPDHH       0x00        //CPU_CLK/4
#define SPDH        0x01        //CPU_CLK/16
#define SPDL        0x02        //CPU_CLK/64
#define SPDLL       0x03        //CPU_CLK/128

void Init_SPI(void);
void SPI_WriteByte( unsigned char input );
unsigned char SPI_ReadByte(void);

#endif /*_SPI_H_*/
