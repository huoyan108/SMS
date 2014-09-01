#pragma once
#include <map>
#include <string>
#include <list>
#include "ComUnit.h"
#include "AlarmClock.h"
#include "PareData.h"
using namespace std;

class CUnitsManager
{
public:
	CUnitsManager();
	~CUnitsManager();
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

	//设置发送信息
	int SetSendMsg(CommReq &Data);

	// 控制设备发送
	int ControlDevSend(char *Dev/*, char *buff, int size*/);

	//接收设备数据
	friend int  RecvDevData(char *DevID, char *buff, int  len);
protected:
	//设备单元集合
	map <string, CComUnit*>m_devMap;

	//返回数据处理
	friend int  FeedBackFun(int nStyle,void *object);

	//到时处理
	friend int  RemindFun(char *pId);

	CAlarmClock m_AlarmClock;

	CPareData m_parse;

	//数据发送列表
	list<CommReq> m_dataList;

	// 向设备单元添加发送信息,如果pData为空，从列表获取
	int SetDevSendMsg(char *DevID, CommReq *pData = NULL);

	// 获取设备
	int GetComDev(CComUnit** comDevPt, char *Dev);

public:
	// IC卡自检
	int SendICJC(char *Dev);
	// 系统自检
	int SendXTZJ(char *Dev, int nZJPD);
};

