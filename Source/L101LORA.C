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

L101ATSTA L101_ATResoponSearch(const uint8_t *pucStr, uint8_t *pucBuf, uint8_t ucBuflen)
{
	uint8_t ucStrlen, *pucIndex,x;
	ucStrlen = strlen(pucStr);
	if(ucStrlen > ucBuflen)
		return L101_AT_E6;

	
	pucIndex = strstr(pucBuf,pucStr);
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
		printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
		return L101_AT_E6;
	}
		
	if (*pucRec=='a')
	{
		S2SendData("a",1);
	}
	else
	{
		printf("%s %s,S2 respone error\r\n",__FILE__,__LINE__);
		return L101_AT_E6;
	}

	do{
		ucLenRec = S2ReadData(pucRec,AT_READ_DELAY);//delay 10ms before read
		if(ucLenRec)
			break;
	}while(--trycnt);

	if(!trycnt)//error
	{
		printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
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
		printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
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
		printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
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
		printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
		return L101_AT_E6;
	}

	if(L101_AT_OK == L101_ATResoponSearch("+NID:",pucRec,ucLenRec))
	{
		sscanf(pucRec,"+NID:%x\r\nOK",ulId);
		printf("UID:%x\r\n",*ulId);
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
		printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
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
		printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
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
		printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
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
		printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
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
		printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
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
		printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
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
		printf("%s %s,S2 no data Rec\r\n",__FILE__,__LINE__);
		return L101_AT_E6;
	}
	
	return L101_ATResoponSearch("OK",pucRec,ucLenRec);
}

L101ATSTA InitL101Module(L101_PARA * xInstPara)
{
	//L101_PARA xdata xL101Inst;
	//xL101Inst.eMode = FP;
	//xL101Inst.eRate = R21875BPS;
	//xL101Inst.ucCh = 
	L101ATSTA eRes;

	eRes = L101_EnterATMode();	//enter AT command mode
	if(eRes != L101_AT_OK)
	{
		printf("Enter AT mode Fail [%d]\r\n",eRes);
		return eRes;
	}

	eRes = L101_SetEcho(OFF); //turn off command echo function
	if(eRes != L101_AT_OK)
	{
		printf("Off Echo Fail [%d]\r\n",eRes);
		return eRes;
	}

	eRes = L101_ReadModuleId(&(xInstPara->ulId)); //read out node ID, this ID can be used for calculation the unique address
	if(eRes != L101_AT_OK)
	{
		printf("Read ID Fail [%d]\r\n",eRes);
		return eRes;
	}

	eRes = L101_ReadModuleVer(); //read firmware version
	if(eRes != L101_AT_OK)
	{
		printf("Read Ver Fail [%d]\r\n",eRes);
		return eRes;
	}

	eRes = L101_SetWorkMode(xInstPara->eMode); //set wroking mode
	if(eRes != L101_AT_OK)
	{
		printf("set mode Fail [%d]\r\n",eRes);
		return eRes;
	}
	
	eRes = L101_SetRate(xInstPara->eRate); //set class rate
	if(eRes != L101_AT_OK)
	{
		printf("set rate Fail [%d]\r\n",eRes);
		return eRes;
	}

	eRes = L101_SetAddress(xInstPara->usAddr); //set adress
	if(eRes != L101_AT_OK)
	{
		printf("set addr Fail [%d]\r\n",eRes);
		return eRes;
	}
	
	eRes = L101_SetChannle(xInstPara->ucCh); //set channel
	if(eRes != L101_AT_OK)
	{
		printf("set addr Fail [%d]\r\n",eRes);
		return eRes;
	}
	
	eRes = L101_SetFec(xInstPara->eFec); //set FEC
	if(eRes != L101_AT_OK)
	{
		printf("set FEC Fail [%d]\r\n",eRes);
		return eRes;
	}
	eRes = L101_SetTxPower(xInstPara->ucPower); //set Tx Power
	if(eRes != L101_AT_OK)
	{
		printf("set Power Fail [%d]\r\n",eRes);
		return eRes;
	}
	
	eRes = L101_ExitATMode(); //exit AT command mode, begin data transmmit
	if(eRes != L101_AT_OK)
	{
		printf("exit AT mode fail [%d]\r\n",eRes);
		return eRes;
	}
	return L101_AT_OK;
}

