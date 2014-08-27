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
	// ���һ����ʱ��
	int AddATimer(char *pId,unsigned long nTime);
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
	map<char *, unsigned long> m_TimerMap;
	//�����߳̾��
	pthread_t m_TimerPt;		

};

