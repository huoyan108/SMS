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
// 添加设备
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


// 删除设备
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


// 修改设备
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


// 控制设备发送
int CComManager::ControlDevSend(char *Dev/*, char *buff, int size*/)
{
	return 0;
}


// 向设备添加发送信息
int CComManager::SetSendMsg()
{
	return 0;
}


// 从发送列表获取发送信息
int CComManager::GetSendMsg()
{
	return 0;
}
