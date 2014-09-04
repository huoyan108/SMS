#pragma once
#include <map>
#include <stdio.h>
#include   <unistd.h>
//#include   <system.h>
#include <pthread.h>
using namespace std;
#define DWORD unsigned long

#define FALSE -1
#define TRUE 0
typedef int(*Remind) (char *pId);
struct ALARMTAG
{
	int nTimeoOutFlag;
	DWORD nDefTime;
	DWORD nLastTime;
};
class CAlarmClock
{
public:
	CAlarmClock();
	~CAlarmClock();
	// 添加一个定时器
	int AddATimer(char *pId,DWORD nTime);
	// 删除一个定时器
	int DelATimer(char *pId);
	// 开启闹钟
	int Start(Remind remindFun);
	// 关闭闹钟
	int Stop();
	// 进行计时
	int ProcessTime();
	//到点提醒
	Remind m_remindFun;
	//定时器集合
	map<char *, ALARMTAG *> m_TimerMap;
	//接收线程句柄
	pthread_t m_TimerPt;		
};

