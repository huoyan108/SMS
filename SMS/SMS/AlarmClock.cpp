#include "AlarmClock.h"

void *threadProcess(void *arg);

CAlarmClock::CAlarmClock() :m_remindFun(0)
{
}


CAlarmClock::~CAlarmClock()
{
}


// 添加一个定时器
int CAlarmClock::AddATimer(char *pId, unsigned long nTime)
{
	printf("AddATimer %s",pId);
	m_TimerMap.insert(map<char *, unsigned long>::value_type(pId, nTime));
	return TRUE;
}


// 删除一个定时器
int CAlarmClock::DelATimer(char *pId)
{
	printf("DelATimer %s", pId);

	m_TimerMap.erase(pId);
	return TRUE;
}


// 开启闹钟
int CAlarmClock::Start(Remind remindFun)
{
	m_remindFun = remindFun;
	//开启线程
	pthread_create(&m_TimerPt, NULL, threadProcess, this);

	return 0;
}

void *threadProcess(void *arg)
{
	pthread_detach(pthread_self());

	int state, oldstate;

	state = PTHREAD_CANCEL_DEFERRED;
	pthread_setcancelstate(state, &oldstate);

	CAlarmClock *me = (CAlarmClock *)arg;
	while (true)
	{
		printf("ProcessTimerThreadID: %d", pthread_self());

		if (me->ProcessTime() != TRUE)
		{
			break;
		}
		pthread_testcancel();

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
	return 0;
}


// 进行计时
int CAlarmClock::ProcessTime()
{
	return 0;
}
