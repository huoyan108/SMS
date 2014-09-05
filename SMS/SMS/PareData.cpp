#include "PareData.h"

typedef struct tagSInfoTitle
{
	int iInfoID;
	char cTitle[6];
}SInfoTitle;

const int INFOCOUNT_BD = 6;
SInfoTitle InfoTitle_BD[] =
{
	{ CUNPACK_DWXX, "DWXX" },
	{ CUNPACK_TXXX, "TXXX" },
	{ CUNPACK_TXHZ, "TXHZ" },
	{ CUNPACK_ICXX, "ICXX" },
	{ CUNPACK_ZJXX, "ZJXX" },
	{ CUNPACK_FKXX, "FKXX" },
};
const int INFOCOUNT_TCP = 3;
SInfoTitle InfoTitle_TCP[] =
{
	{ CUNPACK_TXFS_DS, "TXFS" },
	{ CUNPACK_XTZJ_DS, "XTZJ" },
	{ CUNPACK_XTYD_DS, "XTYD" },
};
char CPareData::ComputeCheckSum(char * Buff, DWORD Len)
{
	char CheckSum = 0;
	for (DWORD i = 0; i<Len; i++)
		CheckSum ^= Buff[i];
	return CheckSum;
}

DWORD RoundMax(DWORD Dividend, DWORD Divisor)
{
	if (Dividend % Divisor != 0)
		return Dividend / Divisor + 1;
	else
		return Dividend / Divisor;
}

CPareData::CPareData()
{
}


CPareData::~CPareData()
{
}
DWORD CPareData::SendToBd_TXSQ(tagBdReq& stReq, char* pSendBuffer, DWORD &nBufferlength)
{
	
	memcpy(pSendBuffer, "$TXSQ", 5);

	DWORD f_iCurLen;
	DWORD f_dwTemp;

	//本机地址
	f_iCurLen = 7;
	pSendBuffer[f_iCurLen] = LOBYTE(HIWORD(stReq.SourceAddress));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = HIBYTE(LOWORD(stReq.SourceAddress));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = LOBYTE(LOWORD(stReq.SourceAddress));
	f_iCurLen++;
	//报文类别
	pSendBuffer[f_iCurLen] = 0x40;
	pSendBuffer[f_iCurLen] |=  0x00; //0
	pSendBuffer[f_iCurLen] |= 0x00;
	pSendBuffer[f_iCurLen] |= stReq.nMsgType ? 0x02 : 0x00;
	f_iCurLen++;
	//目的地址
	pSendBuffer[f_iCurLen] = LOBYTE(HIWORD(stReq.DestAddress));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = HIBYTE(LOWORD(stReq.DestAddress));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = LOBYTE(LOWORD(stReq.DestAddress));
	f_iCurLen++;
	//报文长度
	int nReportLen = stReq.InfoLen;
	if (stReq.nMsgType == 2)
		nReportLen += 8;

	pSendBuffer[f_iCurLen] = HIBYTE(LOWORD(nReportLen));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = LOBYTE(LOWORD(nReportLen));
	f_iCurLen++;	
	//是否应答
	pSendBuffer[f_iCurLen] = 0;
	f_iCurLen++;
	//报文内容
	if (stReq.nMsgType == 2) //混发
	{
		pSendBuffer[f_iCurLen] = 0xA4;
		f_iCurLen++;

		f_dwTemp = RoundMax(nReportLen, 8); //加入混发标志
	}
	else if (stReq.nMsgType == 1 || stReq.nMsgType == 0)
	{
		f_dwTemp = RoundMax(nReportLen, 8);
	}


	memcpy(pSendBuffer + f_iCurLen, stReq.InfoBuff, f_dwTemp);
	f_iCurLen += f_dwTemp;

	pSendBuffer[5] = HIBYTE(LOWORD(f_iCurLen + 1));//长度高字节
	pSendBuffer[6] = LOBYTE(LOWORD(f_iCurLen + 1));//长度低字节

	pSendBuffer[f_iCurLen] = ComputeCheckSum(pSendBuffer, f_iCurLen);
	f_iCurLen++;

	nBufferlength = f_iCurLen;
	return f_iCurLen;
}


char CPareData::ExplainData_BD(char* pSrcBuffer, DWORD dwBufferLen, char* pDestBuffer)
{
	char nReportID;
	int i;

	for (i = 0; i<INFOCOUNT_BD; i++)
	{
		if (memcmp(pSrcBuffer + 1, InfoTitle_BD[i].cTitle, 4) == 0)
		{
			nReportID = InfoTitle_BD[i].iInfoID;
			break;
		}
	}

	if (i >= INFOCOUNT_BD)
		return CUNPACK_NULL;

	int  f_iCurLen;
	DWORD  f_dwTemp;//无符号

	switch (nReportID)
	{
	case CUNPACK_ICXX:
	{
		lpCardInfoRead f_pCardInfoRead;
		f_pCardInfoRead = (lpCardInfoRead)pDestBuffer;

		f_iCurLen = 7;
		//本机地址
		f_dwTemp = pSrcBuffer[f_iCurLen];
		f_dwTemp *= 256;
		f_iCurLen++;
		f_dwTemp += pSrcBuffer[f_iCurLen];
		f_dwTemp *= 256;
		f_iCurLen++;
		f_dwTemp += pSrcBuffer[f_iCurLen];
		f_pCardInfoRead->LocalID = f_dwTemp;
		f_iCurLen++;
		//帪号
		f_pCardInfoRead->FrameNum = pSrcBuffer[f_iCurLen];
		f_iCurLen++;

		if (f_pCardInfoRead->FrameNum == 0) //判断帧号是否为首帧
		{
			//通波ID		
			f_dwTemp = pSrcBuffer[f_iCurLen];
			f_dwTemp *= 256;
			f_iCurLen++;
			f_dwTemp += pSrcBuffer[f_iCurLen];
			f_dwTemp *= 256;
			f_iCurLen++;
			f_dwTemp += pSrcBuffer[f_iCurLen];
			f_pCardInfoRead->BroadcastID = f_dwTemp;
			f_iCurLen++;
			//用户机特征
			f_pCardInfoRead->MachineCharacter = pSrcBuffer[f_iCurLen];
			f_iCurLen++;
			//服务频度
			f_dwTemp = pSrcBuffer[f_iCurLen];
			f_dwTemp *= 256;
			f_iCurLen++;
			f_dwTemp += pSrcBuffer[f_iCurLen];
			f_pCardInfoRead->ServeFreq = (unsigned short)f_dwTemp;
			f_iCurLen++;
			//通信级别
			f_pCardInfoRead->CommuLeave = pSrcBuffer[f_iCurLen];
			f_iCurLen++;
			//加密标志
			f_pCardInfoRead->EncryptFlag = pSrcBuffer[f_iCurLen];
			f_iCurLen++;
			//下级用户总数
			f_dwTemp = pSrcBuffer[f_iCurLen];
			f_dwTemp *= 256;
			f_iCurLen++;
			f_dwTemp += pSrcBuffer[f_iCurLen];
			f_pCardInfoRead->SubordinateNum = (unsigned short)f_dwTemp;
			f_iCurLen++;
		}
		else
		{
			//下级用户ID 数目
			f_pCardInfoRead->CurrFrameSubNum = (dwBufferLen - 12) / 3;
			//下级用户地址
			for (i = 0; i < f_pCardInfoRead->CurrFrameSubNum; i++)   //存储下级用户ID
			{
				f_dwTemp = pSrcBuffer[f_iCurLen];
				f_dwTemp *= 256;
				f_iCurLen++;
				f_dwTemp += pSrcBuffer[f_iCurLen];
				f_dwTemp *= 256;
				f_iCurLen++;
				f_dwTemp += pSrcBuffer[f_iCurLen];
				f_pCardInfoRead->lpSubUserAddrs[i] = f_dwTemp;
				f_iCurLen++;
			}
		}
	}
		return CUNPACK_ICXX;
	case CUNPACK_ZJXX:
	{
		lpSelfCheckInfo f_pSelfCheckInfo;
		f_pSelfCheckInfo = (lpSelfCheckInfo)pDestBuffer;
		f_iCurLen = 7;

		//本机用户地址
		f_dwTemp = pSrcBuffer[f_iCurLen];
		f_dwTemp *= 256;
		f_iCurLen++;
		f_dwTemp += pSrcBuffer[f_iCurLen];
		f_dwTemp *= 256;
		f_iCurLen++;
		f_dwTemp += pSrcBuffer[f_iCurLen];
		f_pSelfCheckInfo->LocalID = f_dwTemp;
		f_iCurLen++;

		//IC卡状态
		f_pSelfCheckInfo->ICCardState = pSrcBuffer[f_iCurLen];
		f_iCurLen++;

		//硬件状态
		f_pSelfCheckInfo->HardState = pSrcBuffer[f_iCurLen];
		f_iCurLen++;

		//电池电量
		f_pSelfCheckInfo->Batterystate = pSrcBuffer[f_iCurLen];
		f_iCurLen++;

		//入站状态
		f_pSelfCheckInfo->InState = pSrcBuffer[f_iCurLen];
		f_iCurLen++;

		//功率信息
		for (i = 0; i < 6; i++)
		{
			f_pSelfCheckInfo->PowerState[i] = pSrcBuffer[f_iCurLen];
			f_iCurLen++;
		}
	}
		return CUNPACK_ZJXX;
	case CUNPACK_FKXX:
	{
		lpFeedbackInfo f_pFeedbackInfo;
		f_pFeedbackInfo = (lpFeedbackInfo)pDestBuffer;  //定义结构指针(没有分配具体地址)
		f_iCurLen = 7;

		//本机用户地址
		f_dwTemp = pSrcBuffer[f_iCurLen];
		f_dwTemp *= 256;
		f_iCurLen++;
		f_dwTemp += pSrcBuffer[f_iCurLen];
		f_dwTemp *= 256;
		f_iCurLen++;
		f_dwTemp += pSrcBuffer[f_iCurLen];
		f_pFeedbackInfo->LocalID = f_dwTemp;
		f_iCurLen++;

		//反馈结果
		f_pFeedbackInfo->FeedResult = pSrcBuffer[f_iCurLen];
		f_iCurLen++;
		//保留信息
		f_pFeedbackInfo->Reserve[0] = pSrcBuffer[f_iCurLen];
		f_iCurLen++;
		f_pFeedbackInfo->Reserve[1] = pSrcBuffer[f_iCurLen];
		f_iCurLen++;
		f_pFeedbackInfo->Reserve[2] = pSrcBuffer[f_iCurLen];
		f_iCurLen++;
		f_pFeedbackInfo->Reserve[3] = pSrcBuffer[f_iCurLen];
		f_iCurLen++;
	}
		return CUNPACK_FKXX;
	}
	return CUNPACK_NULL;
}

DWORD CPareData::SendToBd_ICJC(DWORD dwLocalID, char nFramNo, char* pSendBuffer)
{
	memcpy(pSendBuffer, "$ICJC", 5);

	DWORD dwIndex = 7;
	pSendBuffer[dwIndex] = LOBYTE(HIWORD(dwLocalID));
	dwIndex++;
	pSendBuffer[dwIndex] = HIBYTE(LOWORD(dwLocalID));
	dwIndex++;
	pSendBuffer[dwIndex] = LOBYTE(LOWORD(dwLocalID));
	dwIndex++;
	//帧号
	pSendBuffer[dwIndex] = nFramNo;
	dwIndex++;
	//长度
	pSendBuffer[5] = HIBYTE(LOWORD(dwIndex + 1));
	pSendBuffer[6] = LOBYTE(LOWORD(dwIndex + 1));
	//dwIndex += 2;
	//校验
	pSendBuffer[dwIndex] = ComputeCheckSum(pSendBuffer, dwIndex);
	dwIndex++;

	return dwIndex;
}

DWORD CPareData::SendToBd_XTZJ(DWORD dwLocalID, unsigned short nSelfFre, char* pSendBuffer)
{
	memcpy(pSendBuffer, "$XTZJ", 5);

	DWORD dwIndex = 7;
	//地址
	pSendBuffer[dwIndex] = LOBYTE(HIWORD(dwLocalID));
	dwIndex++;
	pSendBuffer[dwIndex] = HIBYTE(LOWORD(dwLocalID));
	dwIndex++;
	pSendBuffer[dwIndex] = LOBYTE(LOWORD(dwLocalID));
	dwIndex++;
	//频度 
	pSendBuffer[dwIndex] = HIBYTE(nSelfFre);
	dwIndex++;
	pSendBuffer[dwIndex] = LOBYTE(nSelfFre);
	dwIndex++;

	pSendBuffer[5] = HIBYTE(LOWORD(dwIndex + 1));
	pSendBuffer[6] = LOBYTE(LOWORD(dwIndex + 1));


	pSendBuffer[dwIndex] = ComputeCheckSum(pSendBuffer, dwIndex);
	dwIndex++;
	return dwIndex;
}

DWORD CPareData::SendToDS_FKXX(DWORD dwSerialID, char nFeedResult, char* pSendBuffer, DWORD &nBufferlength)
{
	pSendBuffer[0] = '$';
	memcpy(pSendBuffer + 1, "BDFK", 4);

	DWORD f_iCurLen;
	f_iCurLen = 7;

	////指挥机ID
	//sprintf(pSendBuffer + f_iCurLen, "%d", swab32(dwLocalID));
	//f_iCurLen += sizeof(DWORD);
	////目的ID
	//sprintf(pSendBuffer + f_iCurLen, "%d", swab32(dwDestID));
	//f_iCurLen += sizeof(DWORD);
	//通信流水
	sprintf(pSendBuffer + f_iCurLen, "%d", swab32(dwSerialID));
	f_iCurLen += sizeof(DWORD);
	//反馈标志
	pSendBuffer[f_iCurLen] = nFeedResult;
	f_iCurLen++;

	//长度
	//sprintf(pSendBuffer + f_iCurLen, "%d", swab16(f_iCurLen + 1));
	//f_iCurLen += 2;

	pSendBuffer[f_iCurLen] = ComputeCheckSum(pSendBuffer, f_iCurLen);

	f_iCurLen++;

	nBufferlength = f_iCurLen;
	return f_iCurLen;
}


