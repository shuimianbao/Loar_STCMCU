#ifndef _EEPROM_H_
#define _EEPROM_H_

/*Define ISP/IAP/EEPROM Command*/
#define CMD_IDLE    0        //Stand-By
#define CMD_READ    1        //Byte-Read
#define CMD_PROGRAM	2        //Byte-Program
#define CMD_ERASE   3        //Sector-Erase

//定义Flash 操作等待时间及允许IAP/ISP/EEPROM 操作的常数
//#define ENABLE_ISP 0x80 //系统工作时钟<30MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x81 //系统工作时钟<24MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x82 //系统工作时钟<20MHz 时，对IAP_CONTR 寄存器设置此值
#define ENABLE_ISP 0x83 //系统工作时钟<12MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x84 //系统工作时钟<6MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x85 //系统工作时钟<3MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x86 //系统工作时钟<2MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x87 //系统工作时钟<1MHz 时，对IAP_CONTR 寄存器设置此值

void Disable_MCUEeprom (void);
void Write_MCUEeprom(unsigned int Add, unsigned char * Src, unsigned int len);
void Read_MCUEeprom(unsigned int Add, unsigned char * Dest, unsigned int len);
void Sector_Erase(unsigned int Add);

#endif /*_EEPROM_H_*/
