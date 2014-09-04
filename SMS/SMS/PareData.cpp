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
CPareData::CPareData()
{
}


CPareData::~CPareData()
{
}
DWORD CPareData::SendToBd_TXSQ(BdfsMsg& stReq, char* pSendBuffer, DWORD &nBufferlength)
{
	memcpy(pSendBuffer, "$TXSQ", 5);

	DWORD f_iCurLen;
	DWORD f_dwTemp;

	//������ַ
	f_iCurLen = 7;
	pSendBuffer[f_iCurLen] = LOBYTE(HIWORD(stReq.nsourceaddress));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = HIBYTE(LOWORD(stReq.nsourceaddress));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = LOBYTE(LOWORD(stReq.nsourceaddress));
	f_iCurLen++;
	//�������
	pSendBuffer[f_iCurLen] = 0x40;
	pSendBuffer[f_iCurLen] |=  0x00; //0
	pSendBuffer[f_iCurLen] |= 0x00;
	pSendBuffer[f_iCurLen] |= stReq.nmsgtype ? 0x02 : 0x00;
	f_iCurLen++;
	//Ŀ�ĵ�ַ
	pSendBuffer[f_iCurLen] = LOBYTE(HIWORD(stReq.ndestaddress));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = HIBYTE(LOWORD(stReq.ndestaddress));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = LOBYTE(LOWORD(stReq.ndestaddress));
	f_iCurLen++;
	//���ĳ���
	int nReportLen = stReq.ninfolen;
	if (stReq.nmsgtype == 2)
		nReportLen += 8;

	sprintf(pSendBuffer + f_iCurLen, "%d", swab16(nReportLen));
	f_iCurLen += 2;
	//�Ƿ�Ӧ��
	pSendBuffer[f_iCurLen] = 0;
	f_iCurLen++;
	//��������
	if (stReq.nmsgtype == 2) //�췢
	{
		pSendBuffer[f_iCurLen] = 0xA4;
		f_iCurLen++;

		f_dwTemp = RoundMax(nReportLen, 8); //����췢��־
	}
	else if (stReq.nmsgtype == 1 || stReq.nmsgtype == 0)
	{
		f_dwTemp = RoundMax(nReportLen, 8);
	}


	memcpy(pSendBuffer + f_iCurLen, stReq.sinfobuff, f_dwTemp);
	f_iCurLen += f_dwTemp;

	sprintf(pSendBuffer + f_iCurLen, "%d", swab16(f_iCurLen + 1));
	f_iCurLen += 2;

	pSendBuffer[f_iCurLen] = ComputeCheckSum(pSendBuffer, f_iCurLen);
	f_iCurLen++;

	nBufferlength = f_iCurLen;
	return f_iCurLen;
}

DWORD RoundMax(DWORD Dividend, DWORD Divisor)
{
	if (Dividend % Divisor != 0)
		return Dividend / Divisor + 1;
	else
		return Dividend / Divisor;
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
	sprintf(pSendBuffer + dwIndex, "%d", swab16(dwIndex + 1));
	dwIndex += 2;
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
	sprintf(pSendBuffer + dwIndex, "%d", swab16(nSelfFre));
	dwIndex += 2;

	sprintf(pSendBuffer + dwIndex, "%d", swab16(dwIndex + 1));
	dwIndex += 2;

	pSendBuffer[dwIndex] = ComputeCheckSum(pSendBuffer, dwIndex);
	dwIndex++;
	return dwIndex;
}

char CPareData::ExplainData_TCP(char* pSrcBuffer, DWORD dwBufferLen, char* pDestBuffer)
{
	char nReportID;
	int i;

	for (i = 0; i<INFOCOUNT_TCP; i++)
	{
		if (memcmp(pSrcBuffer + 1, InfoTitle_TCP[i].cTitle, 4) == 0)
		{
			nReportID = InfoTitle_TCP[i].iInfoID;
			break;
		}
	}

	if (i >= INFOCOUNT_TCP)
		return CUNPACK_NULL;

	int  f_iCurLen;
	DWORD  f_dwTemp;//�޷���

	switch (nReportID)
	{
	case CUNPACK_TXFS_DS:
	{
		CommReq* pReq;
		pReq = (CommReq*)pDestBuffer;

		f_iCurLen = 7;
		//ͨ����ˮ
		pReq->dwSerialID = pSrcBuffer[f_iCurLen] * 256 * 256 * 256;
		f_iCurLen++;
		pReq->dwSerialID += pSrcBuffer[f_iCurLen] * 256 * 256;
		f_iCurLen++;
		pReq->dwSerialID += pSrcBuffer[f_iCurLen] * 256;
		f_iCurLen++;
		pReq->dwSerialID += pSrcBuffer[f_iCurLen];
		f_iCurLen++;
		//ָ�ӻ�ID
		pReq->LocalID = pSrcBuffer[f_iCurLen] * 256 * 256 * 256;
		f_iCurLen++;
		pReq->LocalID += pSrcBuffer[f_iCurLen] * 256 * 256;
		f_iCurLen++;
		pReq->LocalID += pSrcBuffer[f_iCurLen] * 256;
		f_iCurLen++;
		pReq->LocalID += pSrcBuffer[f_iCurLen];
		f_iCurLen++;
		//���շ�ID
		pReq->DestAddress = pSrcBuffer[f_iCurLen] * 256 * 256 * 256;
		f_iCurLen++;
		pReq->DestAddress += pSrcBuffer[f_iCurLen] * 256 * 256;
		f_iCurLen++;
		pReq->DestAddress += pSrcBuffer[f_iCurLen] * 256;
		f_iCurLen++;
		pReq->DestAddress += pSrcBuffer[f_iCurLen];
		f_iCurLen++;
		//ͨ�ű���
		pReq->ifBCD = pSrcBuffer[f_iCurLen];
		f_iCurLen++;
		//�Ӽ�
		pReq->ifQuick = pSrcBuffer[f_iCurLen];
		f_iCurLen++;
		//��Ϣ����
		pReq->InfoLen = pSrcBuffer[f_iCurLen] * 256;
		f_iCurLen++;
		pReq->InfoLen += pSrcBuffer[f_iCurLen];
		f_iCurLen++;
		//��Ϣ����
		
		//ͨ�������ֽ���
		if (pReq->InfoLen % 8 != 0)
			f_dwTemp =  pReq->InfoLen / 8 + 1;
		else
			f_dwTemp =  pReq->InfoLen / 8;
		//��Ϣ����
		if (f_dwTemp > 210)
			f_dwTemp = 210;

		memcpy(pReq->InfoBuff, pSrcBuffer + f_iCurLen, f_dwTemp);
		pReq->InfoBuff[f_dwTemp] = 0;
	}
		return CUNPACK_TXFS_DS;
	case CUNPACK_XTZJ_DS:
	{
	}
		return CUNPACK_XTZJ_DS;
	case CUNPACK_XTYD_DS:
	{
		tagClientCheck* pCheck;
		pCheck = (tagClientCheck*)pDestBuffer;

		f_iCurLen = 7;
		pCheck->dwAnswerIndex = pSrcBuffer[f_iCurLen] * 256 * 256 * 256;
		f_iCurLen++;
		pCheck->dwAnswerIndex += pSrcBuffer[f_iCurLen] * 256 * 256;
		f_iCurLen++;
		pCheck->dwAnswerIndex += pSrcBuffer[f_iCurLen] * 256;
		f_iCurLen++;
		pCheck->dwAnswerIndex += pSrcBuffer[f_iCurLen];
		f_iCurLen++;
	}
		return CUNPACK_XTYD_DS;
	}
}

DWORD CPareData::SendToDS_XTJC(DWORD dwIndex, char* pSendBuffer)
{
	int f_iCurLen;

	pSendBuffer[0] = '$';
	memcpy(pSendBuffer + 1, "XTJC", 4);

	//������
	f_iCurLen = 7;

	sprintf(pSendBuffer + f_iCurLen, "%d", swab32(dwIndex));
	f_iCurLen += sizeof(DWORD);
	//����
	

	sprintf(pSendBuffer + f_iCurLen, "%d", swab16(f_iCurLen+1));
	f_iCurLen += sizeof(int);

	//У���
	pSendBuffer[f_iCurLen] = ComputeCheckSum(pSendBuffer, f_iCurLen);

	f_iCurLen++;
	return f_iCurLen;
}

DWORD CPareData::SendToDS_DWXX(tagPosInfo& stPosInfo, char* pSendBuffer)
{
	int f_iCurLen;

	pSendBuffer[0] = '$';
	memcpy(pSendBuffer + 1, "DWXX", 4);

	//��λ�û���ID
	f_iCurLen = 7;

	sprintf(pSendBuffer + f_iCurLen, "%d", swab32(stPosInfo.LocalID));
	f_iCurLen += sizeof(int);
	

	//��Կ
	pSendBuffer[f_iCurLen] = stPosInfo.ifSecret;
	f_iCurLen++;
	//����
	pSendBuffer[f_iCurLen] = stPosInfo.Precision;
	f_iCurLen++;
	//�Ӽ�
	pSendBuffer[f_iCurLen] = stPosInfo.ifUrgent;
	f_iCurLen++;
	//��ֵ��
	pSendBuffer[f_iCurLen] = stPosInfo.ifMultiValue;
	f_iCurLen++;
	//ʱ��
	pSendBuffer[f_iCurLen] = stPosInfo.Hour;
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = stPosInfo.Minute;
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = stPosInfo.Second;
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = stPosInfo.CentiSecond;
	f_iCurLen++;
	//����
	pSendBuffer[f_iCurLen] = stPosInfo.LongDeg;
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = stPosInfo.LongMin;
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = stPosInfo.LongSec;
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = stPosInfo.LongDecsec;
	f_iCurLen++;
	//γ��
	pSendBuffer[f_iCurLen] = stPosInfo.LatDeg;
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = stPosInfo.LatMin;
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = stPosInfo.LatSec;
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = stPosInfo.LatDecsec;
	f_iCurLen++;
	//�̷߳���
	pSendBuffer[f_iCurLen] = stPosInfo.nAltSign;
	f_iCurLen++;
	//�߳�
	sprintf(pSendBuffer + f_iCurLen, "%d", swab32(stPosInfo.Altitude));
	f_iCurLen += sizeof(DWORD);

	//�߳��쳣����
	pSendBuffer[f_iCurLen] = stPosInfo.nEfSign;
	f_iCurLen++;
	//�߳��쳣
	sprintf(pSendBuffer + f_iCurLen, "%d", swab16(stPosInfo.Ef));
	f_iCurLen += 2;

	//����
	sprintf(pSendBuffer + f_iCurLen, "%d", swab16(f_iCurLen+1));
	f_iCurLen += 2;

	pSendBuffer[f_iCurLen] = ComputeCheckSum(pSendBuffer, f_iCurLen);
	f_iCurLen++;
	return f_iCurLen;
}

DWORD CPareData::SendToDS_TXXX(tagCommInfo& stCommInfo, char* pSendBuffer)
{
	pSendBuffer[0] = '$';
	memcpy(pSendBuffer + 1, "TXXX", 4);

	DWORD f_iCurLen;

	//�û�����ַ
	f_iCurLen = 7;

	sprintf(pSendBuffer + f_iCurLen, "%d", swab32(stCommInfo.LocalID));
	f_iCurLen += sizeof(DWORD);

	sprintf(pSendBuffer + f_iCurLen, "%d", swab32(stCommInfo.SrcAddress));
	f_iCurLen += sizeof(DWORD);
	//ͨ�ű���
	pSendBuffer[f_iCurLen] = stCommInfo.ifBCD;
	f_iCurLen++;
	//ͨ�����
	pSendBuffer[f_iCurLen] = stCommInfo.ifUrgent;
	f_iCurLen++;
	//���ܱ�־
	pSendBuffer[f_iCurLen] = stCommInfo.ifSecret;
	f_iCurLen++;
	//ʱ
	pSendBuffer[f_iCurLen] = stCommInfo.CommHour;
	f_iCurLen++;
	//��
	pSendBuffer[f_iCurLen] = stCommInfo.CommMin;
	f_iCurLen++;
	//��
	pSendBuffer[f_iCurLen] = stCommInfo.CommSecond;
	f_iCurLen++;
	//���ĳ���
	sprintf(pSendBuffer + f_iCurLen, "%d", swab16(stCommInfo.CommLenBit));
	f_iCurLen += 2;
	//��������
	memcpy(pSendBuffer + f_iCurLen, stCommInfo.CommBuff, stCommInfo.CommLenByte);
	f_iCurLen += stCommInfo.CommLenByte;

	sprintf(pSendBuffer + f_iCurLen, "%d", swab16(f_iCurLen+1));
	f_iCurLen += 2;

	pSendBuffer[f_iCurLen] = ComputeCheckSum(pSendBuffer, f_iCurLen);

	f_iCurLen++;
	return f_iCurLen;
}

DWORD CPareData::SendToDS_TXHZ(tagSendBackInfo& stSendBack, DWORD dwSerialID, char* pSendBuffer)
{
	pSendBuffer[0] = '$';
	memcpy(pSendBuffer + 1, "TXHZ", 4);

	DWORD f_iCurLen;

	//���ͷ���ַ
	f_iCurLen = 7;
	sprintf(pSendBuffer + f_iCurLen, "%d", swab32(stSendBack.dwSendID));
	f_iCurLen += sizeof(DWORD);
	//���շ���ַ
	sprintf(pSendBuffer + f_iCurLen, "%d", swab32(stSendBack.dwRecvID));
	f_iCurLen += sizeof(DWORD);
	//ͨ����ˮ��
	sprintf(pSendBuffer + f_iCurLen, "%d", swab32(dwSerialID));
	f_iCurLen += sizeof(DWORD);
	//ʱ��
	pSendBuffer[f_iCurLen] = stSendBack.nHour;
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = stSendBack.nMinute;
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = stSendBack.nSecond;
	f_iCurLen++;

	//����
	sprintf(pSendBuffer + f_iCurLen, "%d", swab16(f_iCurLen + 1));
	f_iCurLen += 2;

	pSendBuffer[f_iCurLen] = ComputeCheckSum(pSendBuffer, f_iCurLen);
	f_iCurLen++;

	return f_iCurLen;
}

DWORD CPareData::SendToDS_ZJXX(DWORD dwMachineID, char nStatus, char* pSendBuffer)
{
	pSendBuffer[0] = '$';
	memcpy(pSendBuffer + 1, "ZJXX", 4);

	DWORD f_iCurLen;

	//ָ�ӻ�ID
	f_iCurLen = 7;

	sprintf(pSendBuffer + f_iCurLen, "%d", swab32(dwMachineID));
	f_iCurLen += sizeof(DWORD);

	//ָ�ӻ�״̬
	pSendBuffer[f_iCurLen] = nStatus;
	f_iCurLen++;

	//����
	sprintf(pSendBuffer + f_iCurLen, "%d", swab16(f_iCurLen + 1));
	f_iCurLen += 2;

	pSendBuffer[f_iCurLen] = ComputeCheckSum(pSendBuffer, f_iCurLen);

	f_iCurLen++;
	return f_iCurLen;
}

DWORD CPareData::SendToDS_FKXX(DWORD dwLocalID, DWORD dwDestID, DWORD dwSerialID, char nFeedResult, char* pSendBuffer)
{
	pSendBuffer[0] = '$';
	memcpy(pSendBuffer + 1, "BDFK", 4);

	DWORD f_iCurLen;
	//ָ�ӻ�ID
	f_iCurLen = 7;
	sprintf(pSendBuffer + f_iCurLen, "%d", swab32(dwLocalID));
	f_iCurLen += sizeof(DWORD);
	//Ŀ��ID
		sprintf(pSendBuffer + f_iCurLen, "%d", swab32(dwDestID));
	f_iCurLen += sizeof(DWORD);
	//ͨ����ˮ
		sprintf(pSendBuffer + f_iCurLen, "%d", swab32(dwSerialID));
	f_iCurLen += sizeof(DWORD);
	//������־
	pSendBuffer[f_iCurLen] = nFeedResult;
	f_iCurLen++;

	//����
	sprintf(pSendBuffer + f_iCurLen, "%d", swab16(f_iCurLen + 1));
	f_iCurLen += 2;

	pSendBuffer[f_iCurLen] = ComputeCheckSum(pSendBuffer, f_iCurLen);

	f_iCurLen++;
	return f_iCurLen;
}

char CPareData::ExplainData_UDP(char* pSrcBuffer, char* pDestBuffer, DWORD& dwExplainLen)
{
	char nReportID = pSrcBuffer[0];

	DWORD dwIndex = 1;
	DWORD dwTemp = 0;

	switch (nReportID)
	{
	case 0: //��λ��Ϣ
	{
		lpPosInfo f_pPosInfo;
		f_pPosInfo = (lpPosInfo)pDestBuffer;

		//���붨λ�û�ID
		f_pPosInfo->LocalID = pSrcBuffer[dwIndex] * 256 * 256 * 256;
		dwIndex++;
		f_pPosInfo->LocalID += pSrcBuffer[dwIndex] * 256 * 256;
		dwIndex++;
		f_pPosInfo->LocalID += pSrcBuffer[dwIndex] * 256;
		dwIndex++;
		f_pPosInfo->LocalID += pSrcBuffer[dwIndex];
		dwIndex++;
		//�Ƿ����
		f_pPosInfo->ifSecret = pSrcBuffer[dwIndex];
		dwIndex++;
		//��λ����
		f_pPosInfo->Precision = pSrcBuffer[dwIndex];
		dwIndex++;
		//�Ƿ������λ
		f_pPosInfo->ifUrgent = pSrcBuffer[dwIndex];
		dwIndex++;
		//��ֵ��
		f_pPosInfo->ifMultiValue = pSrcBuffer[dwIndex];
		dwIndex++;

		//��Կ
		if (f_pPosInfo->ifSecret)
		{
			memcpy(f_pPosInfo->pSecrets, pSrcBuffer + dwIndex, 6);
			dwIndex += 6;
		}

		/* ʱ�� */
		dwTemp = pSrcBuffer[dwIndex] * 256 * 256;
		dwIndex++;
		dwTemp += pSrcBuffer[dwIndex] * 256;
		dwIndex++;
		dwTemp += pSrcBuffer[dwIndex];
		dwIndex++;
		//ʱ
		f_pPosInfo->Hour = ((dwTemp & 0xF80000) >> 19);
		//��
		f_pPosInfo->Minute = ((dwTemp & 0x07E000) >> 13);
		//��
		f_pPosInfo->Second = ((dwTemp & 0x1F80) >> 7);
		//0.01��
		f_pPosInfo->CentiSecond = (dwTemp & 0x7F);

		/* ���� */
		dwTemp = pSrcBuffer[dwIndex] * 256 * 256;
		dwIndex++;
		dwTemp += pSrcBuffer[dwIndex] * 256;
		dwIndex++;
		dwTemp += pSrcBuffer[dwIndex];
		dwIndex++;
		//��
		f_pPosInfo->LongDeg = ((dwTemp & 0xFF0000) >> 16);
		//��
		f_pPosInfo->LongMin = ((dwTemp & 0xFC00) >> 10);
		//��
		f_pPosInfo->LongSec = ((dwTemp & 0x03F0) >> 4);
		//0.1��
		f_pPosInfo->LongDecsec = (dwTemp & 0x0F);
		/*γ��*/
		dwTemp = pSrcBuffer[dwIndex] * 256 * 256;
		dwIndex++;
		dwTemp += pSrcBuffer[dwIndex] * 256;
		dwIndex++;
		dwTemp += pSrcBuffer[dwIndex];
		dwIndex++;
		//��
		f_pPosInfo->LatDeg = ((dwTemp & 0xFC0000) >> 18);
		//��
		f_pPosInfo->LatMin = ((dwTemp & 0x03F000) >> 12);
		//��
		f_pPosInfo->LatSec = ((dwTemp & 0x0FC0) >> 6);
		//0.1��
		f_pPosInfo->LatDecsec = ((dwTemp & 0x3C) >> 2);
		//��ظ߷���
		f_pPosInfo->nAltSign = ((dwTemp & 0x02) >> 1);
		f_pPosInfo->Altitude = dwTemp & 0x01;
		f_pPosInfo->Altitude = (f_pPosInfo->Altitude << 13);
		//�̼߳��쳣
		dwTemp = pSrcBuffer[dwIndex] * 256 * 256;
		dwIndex++;
		dwTemp += pSrcBuffer[dwIndex] * 256;
		dwIndex++;
		dwTemp += pSrcBuffer[dwIndex];
		dwIndex++;
		//��ظ�
		f_pPosInfo->Altitude |= ((dwTemp & 0xFFF800) >> 11);
		//�߳��쳣����
		f_pPosInfo->nEfSign = ((dwTemp & 0x0400) >> 10);
		//�߳��쳣����
		f_pPosInfo->Ef = ((dwTemp & 0x03FC) >> 2);


		dwExplainLen = dwIndex;
	}
		return CUNPACK_DWXX;
	case 3: //ͨ����Ϣ
	{
		lpCommInfo pCommInfo;
		pCommInfo = (lpCommInfo)pDestBuffer;

		//������ʽ
		pCommInfo->ifBCD = pSrcBuffer[dwIndex];
		dwIndex++;
		//���
		pCommInfo->ifUrgent = pSrcBuffer[dwIndex];
		dwIndex++;
		//�Ƿ����
		pCommInfo->ifSecret = pSrcBuffer[dwIndex];
		dwIndex++;
		//������ַ
		pCommInfo->SrcAddress = pSrcBuffer[dwIndex] * 256 * 256 * 256;
		dwIndex++;
		pCommInfo->SrcAddress += pSrcBuffer[dwIndex] * 256 * 256;
		dwIndex++;
		pCommInfo->SrcAddress += pSrcBuffer[dwIndex] * 256;
		dwIndex++;
		pCommInfo->SrcAddress += pSrcBuffer[dwIndex];
		dwIndex++;
		//�շ���ַ
		pCommInfo->LocalID = pSrcBuffer[dwIndex] * 256 * 256 * 256;
		dwIndex++;
		pCommInfo->LocalID += pSrcBuffer[dwIndex] * 256 * 256;
		dwIndex++;
		pCommInfo->LocalID += pSrcBuffer[dwIndex] * 256;
		dwIndex++;
		pCommInfo->LocalID += pSrcBuffer[dwIndex];
		dwIndex++;
		//ʱ��
		pCommInfo->CommHour = pSrcBuffer[dwIndex];
		dwIndex++;
		pCommInfo->CommMin = pSrcBuffer[dwIndex];
		dwIndex++;
		pCommInfo->CommSecond = pSrcBuffer[dwIndex];
		dwIndex++;
		//���ĳ���
		pCommInfo->CommLenBit = pSrcBuffer[dwIndex] * 256;
		dwIndex++;
		pCommInfo->CommLenBit += pSrcBuffer[dwIndex];
		dwIndex++;
		//��Կ
		if (pCommInfo->ifSecret)
			dwIndex += 6;

		//��������
		if (pSrcBuffer[dwIndex] == 0xA4)
		{
			pCommInfo->ifBCD = 2;
			pCommInfo->CommLenBit -= 8;
			dwIndex++;
		}

		pCommInfo->CommLenByte = RoundMax(pCommInfo->CommLenBit, 8);

		memcpy(pCommInfo->CommBuff, pSrcBuffer + dwIndex, pCommInfo->CommLenByte);
		dwIndex += pCommInfo->CommLenByte;

		dwExplainLen = dwIndex;
	}
		return CUNPACK_TXXX;
	case 4: //ͨ�Ż�ִ
	{
		lpSendBackInfo pSendBack;
		pSendBack = (lpSendBackInfo)pDestBuffer;

		//������ַ
		pSendBack->dwSendID = pSrcBuffer[dwIndex] * 256 * 256 * 256;
		dwIndex++;
		pSendBack->dwSendID += pSrcBuffer[dwIndex] * 256 * 256;
		dwIndex++;
		pSendBack->dwSendID += pSrcBuffer[dwIndex] * 256;
		dwIndex++;
		pSendBack->dwSendID += pSrcBuffer[dwIndex];
		dwIndex++;
		//�շ���ַ
		pSendBack->dwRecvID = pSrcBuffer[dwIndex] * 256 * 256 * 256;
		dwIndex++;
		pSendBack->dwRecvID += pSrcBuffer[dwIndex] * 256 * 256;
		dwIndex++;
		pSendBack->dwRecvID += pSrcBuffer[dwIndex] * 256;
		dwIndex++;
		pSendBack->dwRecvID += pSrcBuffer[dwIndex];
		dwIndex++;
		//ʱ
		pSendBack->nHour = pSrcBuffer[dwIndex];
		dwIndex++;
		pSendBack->nMinute = pSrcBuffer[dwIndex];
		dwIndex++;
		pSendBack->nSecond = pSrcBuffer[dwIndex];
		dwIndex++;

		dwExplainLen = dwIndex;
	}
		return CUNPACK_TXHZ;
	}
}


DWORD CPareData::SendToBd_TXSQ(tagCommReq& stReq, char* pSendBuffer)
{
	memcpy(pSendBuffer, "$TXSQ", 5);

	DWORD f_iCurLen;
	DWORD f_dwTemp;

	//������ַ
	f_iCurLen = 7;
	pSendBuffer[f_iCurLen] = LOBYTE(HIWORD(stReq.LocalID));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = HIBYTE(LOWORD(stReq.LocalID));
	f_iCurLen++;
	pSendBuffer[f_iCurLen] = LOBYTE(LOWORD(stReq.LocalID));
	f_iCurLen++;
	//�������
	pSendBuffer[f_iCurLen] = 0x40;
	pSendBuffer[f_iCurLen] |= stReq.ifSecret ? 0x10 : 0x00; //0
	pSendBuffer[f_iCurLen] |= stReq.ifQuick ? 0x00 : 0x04;
	pSendBuffer[f_iCurLen] |= stReq.ifBCD ? 0x02 : 0x00;
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
	if (stReq.ifBCD == 2)
		nReportLen += 8;

	sprintf(pSendBuffer + f_iCurLen, "%d", swab16(nReportLen));
	f_iCurLen += 2;
	//�Ƿ�Ӧ��
	pSendBuffer[f_iCurLen] = 0;
	f_iCurLen++;
	//��������
	if (stReq.ifBCD == 2) //�췢
	{
		pSendBuffer[f_iCurLen] = 0xA4;
		f_iCurLen++;

		f_dwTemp = RoundMax(nReportLen, 8); //����췢��־
	}
	else
		if (stReq.ifBCD == 1 || stReq.ifBCD == 0)
		{
		f_dwTemp = RoundMax(nReportLen, 8);
		}


	memcpy(pSendBuffer + f_iCurLen, stReq.InfoBuff, f_dwTemp);
	f_iCurLen += f_dwTemp;

	sprintf(pSendBuffer + f_iCurLen, "%d", swab16(f_iCurLen+1));
	f_iCurLen += 2;

	pSendBuffer[f_iCurLen] = ComputeCheckSum(pSendBuffer, f_iCurLen);
	f_iCurLen++;
	return f_iCurLen;
}

char CPareData::ComputeCheckSum(char * Buff, DWORD Len)
{
	char CheckSum = 0;
	for (DWORD i = 0; i<Len; i++)
		CheckSum ^= Buff[i];
	return CheckSum;
}
