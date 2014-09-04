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
#include "./proto/smsTx.pb.h"

class CPareData
{
public:
	CPareData();
	~CPareData();
	DWORD SendToBd_TXSQ(BdfsMsg& stReq, char* pSendBuffer,DWORD &nBufferlength);
	char ExplainData_BD(char* pSrcBuffer, DWORD dwBufferLen, char* pDestBuffer);
	DWORD SendToBd_ICJC(DWORD dwLocalID, char nFramNo, char* pSendBuffer);
	DWORD SendToBd_XTZJ(DWORD dwLocalID, unsigned short nSelfFre, char* pSendBuffer);


	char ExplainData_UDP(char* pSrcBuffer, char* pDestBuffer, DWORD& dwExplainLen);
	char ExplainData_TCP(char* pSrcBuffer, DWORD dwBufferLen, char* pDestBuffer);

	DWORD SendToDS_XTJC(DWORD dwIndex, char* pSendBuffer);
	DWORD SendToDS_DWXX(tagPosInfo& stPosInfo, char* pSendBuffer);
	DWORD SendToDS_TXXX(tagCommInfo& stCommInfo, char* pSendBuffer);
	DWORD SendToDS_TXHZ(tagSendBackInfo& stSendBack, DWORD dwSerialID, char* pSendBuffer);
	DWORD SendToDS_ZJXX(DWORD dwMachineID, char nStatus, char* pSendBuffer);
	DWORD SendToDS_FKXX(DWORD dwLocalID, DWORD dwDestID, DWORD dwSerialID, char nFeedResult, char* pSendBuffer);

	DWORD SendToBd_TXSQ(tagCommReq& stReq, char* pSendBuffer);
protected:
	char ComputeCheckSum(char * Buff, DWORD Len);
};

