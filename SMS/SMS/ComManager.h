#pragma once
#include <map>
#include "ComDev.h"
#include <string>
using namespace std;
class CComManager
{
public:
	CComManager();
	~CComManager();
	/*
	*@nSpeed   类型  int 速率
	*@param  Dev     类型  char  设备路径
	*@param  nBits 类型  int 数据位   取值 为 7 或者8
	*@param  nStop 类型  int 停止位   取值为 1 或者2
	*@param  nEvent  类型  int  效验类型 取值为N,E,O,,S
	*/
	// 添加设备
	int AddComDev(char *Dev,
		int nSpeed,
		int nBits,
		char nEvent,
		int nStop);
	// 删除设备
	int DelComDev(char *Dev);
	// 修改设备 
	int ModifComDev(char *Dev,
		int nSpeed,
		int nBits,
		char nEvent,
		int nStop);

	//接收设备数据
	friend int  RecvDevData(char *DevID, char *buff, int  len);
protected:
	map <string, CComDev*>m_devMap;

	// 控制设备发送
	int ControlDevSend(char *Dev/*, char *buff, int size*/);


public:
	// 向设备添加发送信息
	int SetSendMsg();
	// 从发送列表获取发送信息
	int GetSendMsg();
};

