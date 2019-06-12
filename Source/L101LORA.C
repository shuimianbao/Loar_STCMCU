#include "type.h"


extern uint8_t xdata ucS2RecBuf[S2RECBUFLEN];

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

L101ATSTA L101_ATResoponSearch(const uint8_t *pucStr, uint8_t xdata *pucBuf, uint8_t ucBuflen)
{
	uint8_t ucStrlen, *pucIndex,x,y,index;
	uint8_t xdata *pucDest;
	ucStrlen = strlen(pucStr);
	printf("R0 %bu %bu\r\n",ucStrlen,ucBuflen);
	if(ucStrlen > ucBuflen)
		return L101_AT_E6;
	
	for(x=0; x<ucBuflen;x++)
			printf("%c",pucBuf[x]);
	
	index = pucBuf - ucS2RecBuf; //get the array index
	printf("index %bu\r\n",index);
	for (x = 0; (x < ucBuflen) && ((ucBuflen - x)>=ucStrlen); x++)
	{
		printf("index2 %bu\r\n",(index+x)%S2RECBUFLEN);
		printf("R1 %c %c\r\n",ucS2RecBuf[(index+x)%S2RECBUFLEN],pucStr[0]);
		if(ucS2RecBuf[(index+x)%S2RECBUFLEN]==pucStr[0])//find the first character
		{
			pucDest = pucStr +1;
			y=1;
			while(pucDest)
			{
				
				printf("R2 %c %c\r\n",ucS2RecBuf[(index+x+y)%S2RECBUFLEN],*pucDest);
				if(*pucDest == ucS2RecBuf[(index+x+y)%S2RECBUFLEN])//countiue find character
				{
					y++;
					pucDest++;
				}
				else
					break; //break when character is different
			}
			if(pucBuf == 0) //find the string
				return L101_AT_OK;
		}
	}
	return L101_AT_E6;
/*	
	printf("1");
	pucIndex = strstr(pucBuf,pucStr);
	printf("2");
	if((pucIndex+ucStrlen) <= (pucBuf+ucBuflen))
	{
		pucBuf = pucIndex;
		return L101_AT_OK;
	}
	else
	{
		//Todo: analyse the error
		printf("%s\r\n",pucStr);
		for(x=0; x<ucBuflen;x++)
			printf("%c",pucBuf[x]);
		return L101_AT_E6;
	}
	
*/	
}

L101ATSTA L101_EnterATMode(void)
{
	uint8_t *pucRec;
	uint8_t ucLenRec;
	uint8_t trycnt=MAXTRY;
	//L101ATSTA eResult;
	
	S2SendData("+++",3);
	
	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		//printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
		printf("S2 E1\r\n");
		return L101_AT_E6;
	}
		
	if (*pucRec=='a')
	{
		S2SendData("a",1);
	}
	else
	{
		printf("S2 E2\r\n");
		return L101_AT_E6;
	}

	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("S2 E3\r\n");
		return L101_AT_E6;
	}

	/*
	if(L101_AT_OK == L101_ATResoponSearch("+ok",pucRec,ucLenRec))
		return L101_AT_OK;
	else
		return L101_AT_E6;
	*/
	return L101_ATResoponSearch("+ok",pucRec,ucLenRec);
}

L101ATSTA L101_ExitATMode(void)
{
	uint8_t *pucRec;
	uint8_t ucLenRec;
	uint8_t trycnt=MAXTRY;
	
	S2SendData("AT+ENTM\r\n",9);
	
	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("S2 E4\r\n");
		return L101_AT_E6;
	}
/*
	if(OK == L101_ATResoponSearch("OK",pucRec,ucLenRec))
		return L101_AT_OK;
	else
		return L101_AT_E6;
	*/
	return L101_ATResoponSearch("OK",pucRec,ucLenRec);
}

L101ATSTA L101_SetEcho(ENABLE en)
{
	uint8_t *pucRec;
	uint8_t ucLenRec;
	uint8_t trycnt=MAXTRY;

	if(en==ON)
		S2SendData("AT+E=ON\r\n",9);
	else
		S2SendData("AT+E=OFF\r\n",10);

	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("S2 E5\r\n");
		return L101_AT_E6;
	}

/*
	if(OK == L101_ATResoponSearch("OK",pucRec,ucLenRec))
		return L101_AT_OK;
	else
		return L101_AT_E6;
*/
	return L101_ATResoponSearch("OK",pucRec,ucLenRec);
}

L101ATSTA L101_ReadModuleId(uint32_t *ulId)
{
	uint8_t *pucRec;
	uint8_t ucLenRec;
	uint8_t trycnt=MAXTRY;


	S2SendData("AT+NID\r\n",8);

	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("S2 E6\r\n");
		return L101_AT_E6;
	}

	if(L101_AT_OK == L101_ATResoponSearch("+NID:",pucRec,ucLenRec))
	{
		sscanf(pucRec,"+NID:%lx\r\nOK",ulId);
		printf("UID:%lx\r\n",*ulId);
		return L101_AT_OK;
	}
	else
		return L101_AT_E6;
	
}
L101ATSTA L101_ReadModuleVer(void)
{
	uint8_t  *pucRec;
	uint8_t  ucLenRec;
	uint8_t  trycnt=MAXTRY;
	uint8_t xdata verbuf[30];


	S2SendData("AT+VER\r\n",8);

	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("S2 E7\r\n");
		return L101_AT_E6;
	}

	if(L101_AT_OK == L101_ATResoponSearch("+VER:",pucRec,ucLenRec))
	{
		sscanf(pucRec,"+VER:%s\r\nOK",verbuf);
		printf("VER:%s\r\n",verbuf);
		return L101_AT_OK;
	}
	else
		return L101_AT_E6;
	
}

L101ATSTA L101_SetWorkMode(L101_WROKMODE eMode)
{
	uint8_t  *pucRec;
	uint8_t  ucLenRec;
	uint8_t  trycnt=MAXTRY;

	if(eMode == TRANS)//透传模式
		S2SendData("AT+WMODE=TRANS\r\n",16);
	else
		S2SendData("AT+WMODE=FP\r\n",13);

	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("S2 E8\r\n");
		return L101_AT_E6;
	}
	
	return L101_ATResoponSearch("OK",pucRec,ucLenRec);
}


L101ATSTA L101_SetRate(L101_RATE eRate)
{
	uint8_t  *pucRec;
	uint8_t  ucLenRec;
	uint8_t  ucTmpBuf[20];
	uint8_t  ucTmpLen;
	uint8_t  trycnt=MAXTRY;

	ucTmpLen = sprintf(ucTmpBuf,"AT+SPD=%u\r\n",eRate);
	S2SendData(ucTmpBuf,ucTmpLen);

	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("S2 E9\r\n");
		return L101_AT_E6;
	}
	
	return L101_ATResoponSearch("OK",pucRec,ucLenRec);
}



L101ATSTA L101_SetAddress(uint16_t usAddr)
{
	uint8_t  *pucRec;
	uint8_t  ucLenRec;
	uint8_t  ucTmpBuf[20];
	uint8_t  ucTmpLen;
	uint8_t  trycnt=MAXTRY;

	ucTmpLen = sprintf(ucTmpBuf,"AT+ADDR=%u\r\n",usAddr);
	S2SendData(ucTmpBuf,ucTmpLen);

	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("S2 E10\r\n");
		return L101_AT_E6;
	}
	
	return L101_ATResoponSearch("OK",pucRec,ucLenRec);
}



L101ATSTA L101_SetChannle(uint8_t ucCh)
{
	uint8_t  *pucRec;
	uint8_t  ucLenRec;
	uint8_t  ucTmpBuf[20];
	uint8_t  ucTmpLen;
	uint8_t  trycnt=MAXTRY;

	ucTmpLen = sprintf(ucTmpBuf,"AT+CH=%u\r\n",ucCh);
	S2SendData(ucTmpBuf,ucTmpLen);

	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("S2 E11\r\n");
		return L101_AT_E6;
	}
	
	return L101_ATResoponSearch("OK",pucRec,ucLenRec);
}



L101ATSTA L101_SetFec(ENABLE eEn)
{
	uint8_t  *pucRec;
	uint8_t  ucLenRec;
	uint8_t  ucTmpBuf[20];
	uint8_t  ucTmpLen;
	uint8_t  trycnt=MAXTRY;

	ucTmpLen = sprintf(ucTmpBuf,"AT+CH=%s\r\n",eEn?"OFF":"ON");
	S2SendData(ucTmpBuf,ucTmpLen);

	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("S2 E12\r\n");
		return L101_AT_E6;
	}
	
	return L101_ATResoponSearch("OK",pucRec,ucLenRec);
}



L101ATSTA L101_SetTxPower(uint8_t ucPa)
{
	uint8_t  *pucRec;
	uint8_t  ucLenRec;
	uint8_t  ucTmpBuf[20];
	uint8_t  ucTmpLen;
	uint8_t  trycnt=MAXTRY;

	ucTmpLen = sprintf(ucTmpBuf,"AT+PWR=%u\r\n",ucPa);
	S2SendData(ucTmpBuf,ucTmpLen);

	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("S2 E13\r\n");
		return L101_AT_E6;
	}
	
	return L101_ATResoponSearch("OK",pucRec,ucLenRec);
}
L101ATSTA L101_ReadWelcome(void)
{
	uint8_t xdata *pucRec=0;
	uint8_t ucLenRec,x;
	uint8_t trycnt=MAXTRY;
	printf("A1 0x%x ",pucRec);
	printf(" 0x%x\r\n",&pucRec);
	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY*5);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("S2 E14\r\n");
		return L101_AT_E6;
	}
	
	printf("A2 0x%x",pucRec);
	printf(" 0x%x\r\n",&pucRec);
	
	for(x=0;x<ucLenRec;x++)
			printf("%c",pucRec[x]);
	return L101_ATResoponSearch("LoRa Start!",pucRec,ucLenRec);
}

L101ATSTA InitL101Module(L101_PARA * xInstPara)
{
	//L101_PARA xdata xL101Inst;
	//xL101Inst.eMode = FP;
	//xL101Inst.eRate = R21875BPS;
	//xL101Inst.ucCh = 
	L101ATSTA eRes;

	eRes = L101_ReadWelcome(); //read the welcome word
	if(eRes != L101_AT_OK)
	{
		printf("Read welcome fail [%bu]\r\n",eRes);
		return eRes;
	}
	
	eRes = L101_EnterATMode();	//enter AT command mode
	if(eRes != L101_AT_OK)
	{
		printf("Enter AT mode Fail [%bu]\r\n",eRes);
		return eRes;
	}

	eRes = L101_SetEcho(OFF); //turn off command echo function
	if(eRes != L101_AT_OK)
	{
		printf("Off Echo Fail [%bu]\r\n",eRes);
		return eRes;
	}

	eRes = L101_ReadModuleId(&(xInstPara->ulId)); //read out node ID, this ID can be used for calculation the unique address
	if(eRes != L101_AT_OK)
	{
		printf("Read ID Fail [%bu]\r\n",eRes);
		return eRes;
	}

	eRes = L101_ReadModuleVer(); //read firmware version
	if(eRes != L101_AT_OK)
	{
		printf("Read Ver Fail [%bu]\r\n",eRes);
		return eRes;
	}

	eRes = L101_SetWorkMode(xInstPara->eMode); //set wroking mode
	if(eRes != L101_AT_OK)
	{
		printf("set mode Fail [%bu]\r\n",eRes);
		return eRes;
	}
	
	eRes = L101_SetRate(xInstPara->eRate); //set class rate
	if(eRes != L101_AT_OK)
	{
		printf("set rate Fail [%bu]\r\n",eRes);
		return eRes;
	}

	eRes = L101_SetAddress(xInstPara->usAddr); //set adress
	if(eRes != L101_AT_OK)
	{
		printf("set addr Fail [%bu]\r\n",eRes);
		return eRes;
	}
	
	eRes = L101_SetChannle(xInstPara->ucCh); //set channel
	if(eRes != L101_AT_OK)
	{
		printf("set addr Fail [%bu]\r\n",eRes);
		return eRes;
	}
	
	eRes = L101_SetFec(xInstPara->eFec); //set FEC
	if(eRes != L101_AT_OK)
	{
		printf("set FEC Fail [%bu]\r\n",eRes);
		return eRes;
	}
	eRes = L101_SetTxPower(xInstPara->ucPower); //set Tx Power
	if(eRes != L101_AT_OK)
	{
		printf("set Power Fail [%bu]\r\n",eRes);
		return eRes;
	}
	
	eRes = L101_ExitATMode(); //exit AT command mode, begin data transmmit
	if(eRes != L101_AT_OK)
	{
		printf("exit AT mode fail [%bu]\r\n",eRes);
		return eRes;
	}
	return L101_AT_OK;
}

L101ATSTA L101_SendWithTransMode(uint8_t *pucSendbuf,uint8_t ucLen)
{
	S2SendData(pucSendbuf,ucLen);
	return L101_AT_OK;
}
L101ATSTA L101_SendWithFPMode(uint8_t *pucSendbuf,uint8_t ucLen,uint8_t ucCh, uint16_t usAddr)
{
	uint8_t ucTmpbuf[3];
	ucTmpbuf[0]=(uint8_t)(usAddr>>8);
	ucTmpbuf[1]=(uint8_t)usAddr;
	ucTmpbuf[1]=ucCh;
	S2SendData(ucTmpbuf,3);
	S2SendData(pucSendbuf,ucLen);
	return L101_AT_OK;
}

uint8_t L101_Receive(uint8_t *pucRecbuf)
{
	uint8_t ucTmplen = 0;

	ucTmplen = S2ReadData(pucRecbuf, 1);
	if(ucTmplen)//the uart begin receive data
	{
		ucTmplen += S2ReadData(pucRecbuf+ucTmplen, 3);//delay 30ms before read all the data,max data should less than 252
	}

	return ucTmplen;
	
}




