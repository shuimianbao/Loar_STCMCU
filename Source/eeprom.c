#include "type.h"


void Disable_MCUEeprom (void)
{
	//关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    IAP_CONTR = 0;      //关闭IAP 功能
    IAP_CMD   = 0;      //清命令寄存器,使命令寄存器无命令,此句可不用
    IAP_TRIG  = 0;      //清命令触发寄存器,使命令触发寄存器无触发,此句可不用
    IAP_ADDRH = 0;
    IAP_ADDRL = 0;
}

void Write_MCUEeprom(uint16_t Add, uint8_t * Src, uint16_t len)
{
	uint16_t i;
	IAP_CONTR = ENABLE_ISP;         //打开 IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = CMD_PROGRAM;                 //IAP/ISP/EEPROM 字节编程命令
	
	for(i=0; i < len; i++)
	{
		IAP_ADDRH = (uint8_t)((Add+i)>>4);    //设置目标单元地址的高8 位地址
    	IAP_ADDRL = (uint8_t)(Add+i);    //设置目标单元地址的低8 位地址
	
	    IAP_DATA = Src[i];                  //要编程的数据先送进IAP_DATA 寄存器
 
 	   IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    	IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    	_nop_();
    }
	
    Disable_MCUEeprom();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

void Read_MCUEeprom(uint16_t Add, uint8_t * Dest, uint16_t len)
{
	uint16_t i;
	IAP_DATA = 0x00;
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = CMD_READ;                 //IAP/ISP/EEPROM 字节读命令

   for(i=0; i < len; i++)
	{
		IAP_ADDRH = (uint8_t)((Add+i)>>4);    //设置目标单元地址的高8 位地址
    	IAP_ADDRL = (uint8_t)(Add+i);    //设置目标单元地址的低8 位地址
 
 	   	IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    	IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
		
		Dest[i] = IAP_DATA;        //读取的数据放入缓冲区中
    	_nop_();
    }
	
    Disable_MCUEeprom();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}


void Sector_Erase(uint16_t Add)
{
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = CMD_ERASE;                 //IAP/ISP/EEPROM 扇区擦除命令

   	IAP_ADDRH = (uint8_t)(Add>>4);    //设置目标单元地址的高8 位地址
   	IAP_ADDRL = (uint8_t)(Add);    //设置目标单元地址的低8 位地址

    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();

    Disable_MCUEeprom();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}
