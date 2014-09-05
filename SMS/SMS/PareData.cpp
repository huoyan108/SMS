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

	//������ַ
	f_iCurLen = 7;
	pSendBuffer[f_iCurLen] = LOBYTE(HIWORD(stReq.SourceAddress));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = HIBYTE(LOWORD(stReq.SourceAddress));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = LOBYTE(LOWORD(stReq.SourceAddress));
	f_iCurLen++;
	//�������
	pSendBuffer[f_iCurLen] = 0x40;
	pSendBuffer[f_iCurLen] |=  0x00; //0
	pSendBuffer[f_iCurLen] |= 0x00;
	pSendBuffer[f_iCurLen] |= stReq.nMsgType ? 0x02 : 0x00;
	f_iCurLen++;
	//Ŀ�ĵ�ַ
	pSendBuffer[f_iCurLen] = LOBYTE(HIWORD(stReq.DestAddress));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = HIBYTE(LOWORD(stReq.DestAddress));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = LOBYTE(LOWORD(stReq.DestAddress));
	f_iCurLen++;
	//���ĳ���
	int nReportLen = stReq.InfoLen;
	if (stReq.nMsgType == 2)
		nReportLen += 8;

	pSendBuffer[f_iCurLen] = HIBYTE(LOWORD(nReportLen));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = LOBYTE(LOWORD(nReportLen));
	f_iCurLen++;	
	//�Ƿ�Ӧ��
	pSendBuffer[f_iCurLen] = 0;
	f_iCurLen++;
	//��������
	if (stReq.nMsgType == 2) //�췢
	{
		pSendBuffer[f_iCurLen] = 0xA4;
		f_iCurLen++;

		f_dwTemp = RoundMax(nReportLen, 8); //����췢��־
	}
	else if (stReq.nMsgType == 1 || stReq.nMsgType == 0)
	{
		f_dwTemp = RoundMax(nReportLen, 8);
	}


	memcpy(pSendBuffer + f_iCurLen, stReq.InfoBuff, f_dwTemp);
	f_iCurLen += f_dwTemp;

	pSendBuffer[5] = HIBYTE(LOWORD(f_iCurLen + 1));//���ȸ��ֽ�
	pSendBuffer[6] = LOBYTE(LOWORD(f_iCurLen + 1));//���ȵ��ֽ�

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
	DWORD  f_dwTemp;//�޷���

	switch (nReportID)
	{
	case CUNPACK_ICXX:
	{
		lpCardInfoRead f_pCardInfoRead;
		f_pCardInfoRead = (lpCardInfoRead)pDestBuffer;

		f_iCurLen = 7;
		//������ַ
		f_dwTemp = pSrcBuffer[f_iCurLen];
		f_dwTemp *= 256;
		f_iCurLen++;
		f_dwTemp += pSrcBuffer[f_iCurLen];
		f_dwTemp *= 256;
		f_iCurLen++;
		f_dwTemp += pSrcBuffer[f_iCurLen];
		f_pCardInfoRead->LocalID = f_dwTemp;
		f_iCurLen++;
		//����
		f_pCardInfoRead->FrameNum = pSrcBuffer[f_iCurLen];
		f_iCurLen++;

		if (f_pCardInfoRead->FrameNum == 0) //�ж�֡���Ƿ�Ϊ��֡
		{
			//ͨ��ID		
			f_dwTemp = pSrcBuffer[f_iCurLen];
			f_dwTemp *= 256;
			f_iCurLen++;
			f_dwTemp += pSrcBuffer[f_iCurLen];
			f_dwTemp *= 256;
			f_iCurLen++;
			f_dwTemp += pSrcBuffer[f_iCurLen];
			f_pCardInfoRead->BroadcastID = f_dwTemp;
			f_iCurLen++;
			//�û�������
			f_pCardInfoRead->MachineCharacter = pSrcBuffer[f_iCurLen];
			f_iCurLen++;
			//����Ƶ��
			f_dwTemp = pSrcBuffer[f_iCurLen];
			f_dwTemp *= 256;
			f_iCurLen++;
			f_dwTemp += pSrcBuffer[f_iCurLen];
			f_pCardInfoRead->ServeFreq = (unsigned short)f_dwTemp;
			f_iCurLen++;
			//ͨ�ż���
			f_pCardInfoRead->CommuLeave = pSrcBuffer[f_iCurLen];
			f_iCurLen++;
			//���ܱ�־
			f_pCardInfoRead->EncryptFlag = pSrcBuffer[f_iCurLen];
			f_iCurLen++;
			//�¼��û�����
			f_dwTemp = pSrcBuffer[f_iCurLen];
			f_dwTemp *= 256;
			f_iCurLen++;
			f_dwTemp += pSrcBuffer[f_iCurLen];
			f_pCardInfoRead->SubordinateNum = (unsigned short)f_dwTemp;
			f_iCurLen++;
		}
		else
		{
			//�¼��û�ID ��Ŀ
			f_pCardInfoRead->CurrFrameSubNum = (dwBufferLen - 12) / 3;
			//�¼��û���ַ
			for (i = 0; i < f_pCardInfoRead->CurrFrameSubNum; i++)   //�洢�¼��û�ID
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

		//�����û���ַ
		f_dwTemp = pSrcBuffer[f_iCurLen];
		f_dwTemp *= 256;
		f_iCurLen++;
		f_dwTemp += pSrcBuffer[f_iCurLen];
		f_dwTemp *= 256;
		f_iCurLen++;
		f_dwTemp += pSrcBuffer[f_iCurLen];
		f_pSelfCheckInfo->LocalID = f_dwTemp;
		f_iCurLen++;

		//IC��״̬
		f_pSelfCheckInfo->ICCardState = pSrcBuffer[f_iCurLen];
		f_iCurLen++;

		//Ӳ��״̬
		f_pSelfCheckInfo->HardState = pSrcBuffer[f_iCurLen];
		f_iCurLen++;

		//��ص���
		f_pSelfCheckInfo->Batterystate = pSrcBuffer[f_iCurLen];
		f_iCurLen++;

		//��վ״̬
		f_pSelfCheckInfo->InState = pSrcBuffer[f_iCurLen];
		f_iCurLen++;

		//������Ϣ
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
		f_pFeedbackInfo = (lpFeedbackInfo)pDestBuffer;  //����ṹָ��(û�з�������ַ)
		f_iCurLen = 7;

		//�����û���ַ
		f_dwTemp = pSrcBuffer[f_iCurLen];
		f_dwTemp *= 256;
		f_iCurLen++;
		f_dwTemp += pSrcBuffer[f_iCurLen];
		f_dwTemp *= 256;
		f_iCurLen++;
		f_dwTemp += pSrcBuffer[f_iCurLen];
		f_pFeedbackInfo->LocalID = f_dwTemp;
		f_iCurLen++;

		//�������
		f_pFeedbackInfo->FeedResult = pSrcBuffer[f_iCurLen];
		f_iCurLen++;
		//������Ϣ
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
	//֡��
	pSendBuffer[dwIndex] = nFramNo;
	dwIndex++;
	//����
	pSendBuffer[5] = HIBYTE(LOWORD(dwIndex + 1));
	pSendBuffer[6] = LOBYTE(LOWORD(dwIndex + 1));
	//dwIndex += 2;
	//У��
	pSendBuffer[dwIndex] = ComputeCheckSum(pSendBuffer, dwIndex);
	dwIndex++;

	return dwIndex;
}

DWORD CPareData::SendToBd_XTZJ(DWORD dwLocalID, unsigned short nSelfFre, char* pSendBuffer)
{
	memcpy(pSendBuffer, "$XTZJ", 5);

	DWORD dwIndex = 7;
	//��ַ
	pSendBuffer[dwIndex] = LOBYTE(HIWORD(dwLocalID));
	dwIndex++;
	pSendBuffer[dwIndex] = HIBYTE(LOWORD(dwLocalID));
	dwIndex++;
	pSendBuffer[dwIndex] = LOBYTE(LOWORD(dwLocalID));
	dwIndex++;
	//Ƶ�� 
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

	////ָ�ӻ�ID
	//sprintf(pSendBuffer + f_iCurLen, "%d", swab32(dwLocalID));
	//f_iCurLen += sizeof(DWORD);
	////Ŀ��ID
	//sprintf(pSendBuffer + f_iCurLen, "%d", swab32(dwDestID));
	//f_iCurLen += sizeof(DWORD);
	//ͨ����ˮ
	sprintf(pSendBuffer + f_iCurLen, "%d", swab32(dwSerialID));
	f_iCurLen += sizeof(DWORD);
	//������־
	pSendBuffer[f_iCurLen] = nFeedResult;
	f_iCurLen++;

	//����
	//sprintf(pSendBuffer + f_iCurLen, "%d", swab16(f_iCurLen + 1));
	//f_iCurLen += 2;

	pSendBuffer[f_iCurLen] = ComputeCheckSum(pSendBuffer, f_iCurLen);

	f_iCurLen++;

	nBufferlength = f_iCurLen;
	return f_iCurLen;
}


