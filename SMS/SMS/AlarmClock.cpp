#include "AlarmClock.h"

void *threadProcess(void *arg);

CAlarmClock::CAlarmClock() :m_remindFun(0)
{
}


CAlarmClock::~CAlarmClock()
{
}


// ���һ����ʱ��
int CAlarmClock::AddATimer(char *pId, unsigned long nTime)
{
	printf("AddATimer %s",pId);
	m_TimerMap.insert(map<char *, unsigned long>::value_type(pId, nTime));
	return TRUE;
}


// ɾ��һ����ʱ��
int CAlarmClock::DelATimer(char *pId)
{
	printf("DelATimer %s", pId);

	m_TimerMap.erase(pId);
	return TRUE;
}


// ��������
int CAlarmClock::Start(Remind remindFun)
{
	m_remindFun = remindFun;
	//�����߳�
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
// �ر�����
int CAlarmClock::Stop()
{
	//��ֹ�߳�
	void * res;
	pthread_cancel(m_TimerPt);
	pthread_join(m_TimerPt, &res);
	printf("stop TIMER thread\n");
	return 0;
}


// ���м�ʱ
int CAlarmClock::ProcessTime()
{
	return 0;
}
