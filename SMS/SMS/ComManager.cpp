#include "ComManager.h"

CComManager *g_pComManager = NULL;
CComManager::CComManager()
{
	g_pComManager = this;
}


CComManager::~CComManager()
{
}

int  RecvDevData(char *DevID, char *buff, int  len)
{
	printf("%s RECV Notif\n",DevID);
	//g_pComManager->m_devMap["ad"];
	return true;
}
// ����豸
int CComManager::AddComDev(char *Dev,
	int nSpeed,
	int nBits,
	char nEvent,
	int nStop)
{
	CComDev *comDevPt = new CComDev;

	comDevPt->SetParam(Dev, nSpeed, nBits, nEvent, nStop, RecvDevData);

	if (comDevPt->Start() != TRUE)
	{
		return FALSE;
	}

	m_devMap[Dev] = comDevPt;
	return TRUE;
}


// ɾ���豸
int CComManager::DelComDev(char *Dev)
{
	map<string, CComDev*>::iterator it = m_devMap.find(Dev);
	if (it != m_devMap.end())
	{
		CComDev *comDevPt = it->second;
		comDevPt->Stop();
		m_devMap.erase(it);
	}
	return TRUE;
}


// �޸��豸
int CComManager::ModifComDev(char *Dev,
	int nSpeed,
	int nBits,
	char nEvent,
	int nStop)
{
	if (DelComDev(Dev) == FALSE)
	{
		return FALSE;
	}
	if (AddComDev(Dev, nSpeed, nBits, nEvent, nStop) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}


// �����豸����
int CComManager::ControlDevSend(char *Dev/*, char *buff, int size*/)
{
	return 0;
}


// ���豸��ӷ�����Ϣ
int CComManager::SetSendMsg()
{
	return 0;
}


// �ӷ����б��ȡ������Ϣ
int CComManager::GetSendMsg()
{
	return 0;
}
