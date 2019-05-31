#include "type.h"


void Disable_MCUEeprom (void)
{
	//�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
    IAP_CONTR = 0;      //�ر�IAP ����
    IAP_CMD   = 0;      //������Ĵ���,ʹ����Ĵ���������,�˾�ɲ���
    IAP_TRIG  = 0;      //��������Ĵ���,ʹ������Ĵ����޴���,�˾�ɲ���
    IAP_ADDRH = 0;
    IAP_ADDRL = 0;
}

void Write_MCUEeprom(unsigned int Add, unsigned char * Src, unsigned int len)
{
	unsigned int i;
	IAP_CONTR = ENABLE_ISP;         //�� IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = CMD_PROGRAM;                 //IAP/ISP/EEPROM �ֽڱ������
	
	for(i=0; i < len; i++)
	{
		IAP_ADDRH = (unsigned char)((Add+i)>>4);    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    	IAP_ADDRL = (unsigned char)(Add+i);    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ
	
	    IAP_DATA = Src[i];                  //Ҫ��̵��������ͽ�IAP_DATA �Ĵ���
 
 	   IAP_TRIG = 0x5A;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    	IAP_TRIG = 0xA5;   //����A5h ��ISP/IAP ����������������
    	_nop_();
    }
	
    Disable_MCUEeprom();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}

void Read_MCUEeprom(unsigned int Add, unsigned char * Dest, unsigned int len)
{
	unsigned int i;
	IAP_DATA = 0x00;
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = CMD_READ;                 //IAP/ISP/EEPROM �ֽڶ�����

   for(i=0; i < len; i++)
	{
		IAP_ADDRH = (unsigned char)((Add+i)>>4);    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    	IAP_ADDRL = (unsigned char)(Add+i);    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ
 
 	   	IAP_TRIG = 0x5A;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    	IAP_TRIG = 0xA5;   //����A5h ��ISP/IAP ����������������
		
		Dest[i] = IAP_DATA;        //��ȡ�����ݷ��뻺������
    	_nop_();
    }
	
    Disable_MCUEeprom();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}


void Sector_Erase(unsigned int Add)
{
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = CMD_ERASE;                 //IAP/ISP/EEPROM ������������

   	IAP_ADDRH = (unsigned char)(Add>>4);    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
   	IAP_ADDRL = (unsigned char)(Add);    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ

    IAP_TRIG = 0x5A;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = 0xA5;   //����A5h ��ISP/IAP ����������������
    _nop_();

    Disable_MCUEeprom();  //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}
