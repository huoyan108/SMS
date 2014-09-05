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
	DWORD SendToBd_TXSQ(tagBdReq& stReq, char* pSendBuffer,DWORD &nBufferlength);
	char ExplainData_BD(char* pSrcBuffer, DWORD dwBufferLen, char* pDestBuffer);
	DWORD SendToBd_ICJC(DWORD dwLocalID, char nFramNo, char* pSendBuffer);
	DWORD SendToBd_XTZJ(DWORD dwLocalID, unsigned short nSelfFre, char* pSendBuffer);
	DWORD SendToDS_FKXX(DWORD dwSerialID, char nFeedResult, char* pSendBuffer,DWORD &nBufferlength);
	char ComputeCheckSum(char * Buff, DWORD Len);


};

