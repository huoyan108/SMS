#pragma once
/*
作者 白彦聪
日期	20140825
说明	解析数据。
*/
#include "MsgStructInfo.h"
#include <string.h>
#include "CNDef.h"
#include <stdio.h>

class CPareData
{
public:
	CPareData();
	~CPareData();

	char ExplainData_BD(char* pSrcBuffer, unsigned long dwBufferLen, char* pDestBuffer);
	char ExplainData_UDP(char* pSrcBuffer, char* pDestBuffer, unsigned long& dwExplainLen);
	char ExplainData_TCP(char* pSrcBuffer, unsigned long dwBufferLen, char* pDestBuffer);

	unsigned long SendToDS_XTJC(unsigned long dwIndex, char* pSendBuffer);
	unsigned long SendToDS_DWXX(tagPosInfo& stPosInfo, char* pSendBuffer);
	unsigned long SendToDS_TXXX(tagCommInfo& stCommInfo, char* pSendBuffer);
	unsigned long SendToDS_TXHZ(tagSendBackInfo& stSendBack, unsigned long dwSerialID, char* pSendBuffer);
	unsigned long SendToDS_ZJXX(unsigned long dwMachineID, char nStatus, char* pSendBuffer);
	unsigned long SendToDS_FKXX(unsigned long dwLocalID, unsigned long dwDestID, unsigned long dwSerialID, char nFeedResult, char* pSendBuffer);

	unsigned long SendToBd_ICJC(unsigned long dwLocalID, char nFramNo, char* pSendBuffer);
	unsigned long SendToBd_XTZJ(unsigned long dwLocalID, unsigned short nSelfFre, char* pSendBuffer);
	unsigned long SendToBd_TXSQ(tagCommReq& stReq, char* pSendBuffer);
protected:
	char ComputeCheckSum(char * Buff, unsigned long Len);
};

