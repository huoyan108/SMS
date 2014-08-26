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
	*@nSpeed   类型  int 速率
	*@param  Dev     类型  char  设备路径
	*@param  nBits 类型  int 数据位   取值 为 7 或者8
	*@param  nStop 类型  int 停止位   取值为 1 或者2
	*@param  nEvent  类型  int  效验类型 取值为N,E,O,,S
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

	//发送列表
	map<int, _SEND_DATA_TAG> m_sendMsgList;
};

