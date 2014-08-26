#include "ComUnit.h"


CComUnit::CComUnit()
{

}

CComUnit::~CComUnit()
{
}

int CComUnit::Start(char *Dev,
	int nSpeed,
	int nBits,
	char nEvent,
	int nStop)
{
	m_comDev.SetParam(Dev, nSpeed, nBits, nEvent, nStop, RecvDevData);
	m_comDev.Start();
	if (m_comDev.Start() != TRUE)
	{
		return FALSE;
	}
	return TRUE;
}
int CComUnit::Stop()
{
	m_comDev.Stop();
	return TRUE;
}

int  RecvDevData(char *DevID, char *buff, int  len)
{
	printf("%s RECV Notif\n", DevID);
	//g_pComManager->m_devMap["ad"];
	return true;
}