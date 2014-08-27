#pragma once
#include <map>
#include <stdio.h>
#include <pthread.h>
using namespace std;

#define FALSE -1
#define TRUE 0
typedef int(*Remind) (char *pId);
class CAlarmClock
{
public:
	CAlarmClock();
	~CAlarmClock();
	// 添加一个定时器
	int AddATimer(char *pId,unsigned long nTime);
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
	map<char *, unsigned long> m_TimerMap;
	//接收线程句柄
	pthread_t m_TimerPt;		

};

