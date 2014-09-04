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
	// ���һ����ʱ��
	int AddATimer(char *pId,DWORD nTime);
	// ɾ��һ����ʱ��
	int DelATimer(char *pId);
	// ��������
	int Start(Remind remindFun);
	// �ر�����
	int Stop();
	// ���м�ʱ
	int ProcessTime();
	//��������
	Remind m_remindFun;
	//��ʱ������
	map<char *, ALARMTAG *> m_TimerMap;
	//�����߳̾��
	pthread_t m_TimerPt;		
};

