#include "AlarmClock.h"

void *threadProcess(void *arg);
//互斥锁
pthread_mutex_t g_alarm_mutex = PTHREAD_MUTEX_INITIALIZER;

CAlarmClock::CAlarmClock() :m_remindFun(0)
{
}


CAlarmClock::~CAlarmClock()
{
}


// 添加一个定时器
int CAlarmClock::AddATimer(char *pId, DWORD nTime)
{
	printf("AddATimer %s\n",pId);
	ALARMTAG *pAlarmTag = new ALARMTAG();
	pAlarmTag->nTimeoOutFlag = -1;
	pAlarmTag->nDefTime = nTime;
	
	pthread_mutex_lock(&g_alarm_mutex);
	m_TimerMap.insert(map<char *, ALARMTAG *>::value_type(pId, pAlarmTag));
	pthread_mutex_unlock(&g_alarm_mutex);
	return TRUE;
}


// 删除一个定时器
int CAlarmClock::DelATimer(char *pId)
{
	printf("DelATimer %s\n", pId);

	pthread_mutex_lock(&g_alarm_mutex);
	map<char *, ALARMTAG*>::iterator it = m_TimerMap.find(pId);
	if (it != m_TimerMap.end())
	{
		delete it->second;
		m_TimerMap.erase(pId);
	}
	pthread_mutex_unlock(&g_alarm_mutex);

	return TRUE;
}


// 开启闹钟
int CAlarmClock::Start(Remind remindFun)
{
	m_remindFun = remindFun;
	//开启线程
	pthread_create(&m_TimerPt, NULL, threadProcess, this);

	return TRUE;
}

void *threadProcess(void *arg)
{
	pthread_detach(pthread_self());
	//printf("ProcessTimerThreadID: %ld\n", pthread_self());

	//int state, oldstate;

	//state = PTHREAD_CANCEL_DEFERRED;
	//pthread_setcancelstate(state, &oldstate);

	CAlarmClock *me = (CAlarmClock *)arg;
	while (true)
	{

		if (me->ProcessTime() != TRUE)
		{
			break;
		}
		pthread_testcancel();
		//sleep(10);
		sleep(1);
	}
	return NULL;
}
// 关闭闹钟
int CAlarmClock::Stop()
{
	//终止线程
	void * res;
	pthread_cancel(m_TimerPt);
	pthread_join(m_TimerPt, &res);
	printf("stop TIMER thread\n");
	return TRUE;
}


// 进行计时
int CAlarmClock::ProcessTime()
{
	pthread_mutex_lock(&g_alarm_mutex);

	map<char *, ALARMTAG *>::iterator it = m_TimerMap.begin();
	for (; it != m_TimerMap.end(); it++)
	{
		
		it->second->nLastTime += 1;
		DWORD nLastTimeTemp = it->second->nLastTime;
		DWORD nDefTime = it->second->nDefTime;
		
		//if (it->second->nDefTime <= it->second->nLastTime)
		if (nDefTime <= nLastTimeTemp)
		{
			m_remindFun(it->first);
			it->second->nLastTime = 0;
		}
	}

	pthread_mutex_unlock(&g_alarm_mutex);

	return TRUE;
}


// 修改定时器
int CAlarmClock::ModifTimer(char *pId, DWORD nTime)
{
	printf("ModifTimer %s\n", pId);

	pthread_mutex_lock(&g_alarm_mutex);
	map<char *, ALARMTAG*>::iterator it = m_TimerMap.find(pId);
	if (it != m_TimerMap.end())
	{
		ALARMTAG *pAlarmTag = it->second;
		pAlarmTag->nTimeoOutFlag = -1;
		pAlarmTag->nDefTime = nTime;
	}
	pthread_mutex_unlock(&g_alarm_mutex);
	return 0;
}
