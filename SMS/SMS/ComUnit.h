#pragma once
#include <map>
#include "ComDev.h"
#include "PareData.h"
#include "MsgStructInfo.h"
using namespace std;

class CComUnit
{
public:
	/*
	*@nSpeed   ����  int ����
	*@param  Dev     ����  char  �豸·��
	*@param  nBits ����  int ����λ   ȡֵ Ϊ 7 ����8
	*@param  nStop ����  int ֹͣλ   ȡֵΪ 1 ����2
	*@param  nEvent  ����  int  Ч������ ȡֵΪN,E,O,,S
	*/
	int Start(char *Dev,
		int nSpeed,
		int nBits,
		char nEvent,
		int nStop);

	int Stop();

	CComUnit();
	~CComUnit();


protected:
	friend int  RecvDevData(char *DevID, char *buff, int  len);

	CComDev m_comDev;

	CPareData parseData;

	//�����б�
	map<int, _SEND_DATA_TAG> m_sendMsgList;
};

