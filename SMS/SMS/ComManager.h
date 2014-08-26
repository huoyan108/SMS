#pragma once
#include <map>
#include "ComDev.h"
#include <string>
using namespace std;
class CComManager
{
public:
	CComManager();
	~CComManager();
	/*
	*@nSpeed   ����  int ����
	*@param  Dev     ����  char  �豸·��
	*@param  nBits ����  int ����λ   ȡֵ Ϊ 7 ����8
	*@param  nStop ����  int ֹͣλ   ȡֵΪ 1 ����2
	*@param  nEvent  ����  int  Ч������ ȡֵΪN,E,O,,S
	*/
	// ����豸
	int AddComDev(char *Dev,
		int nSpeed,
		int nBits,
		char nEvent,
		int nStop);
	// ɾ���豸
	int DelComDev(char *Dev);
	// �޸��豸 
	int ModifComDev(char *Dev,
		int nSpeed,
		int nBits,
		char nEvent,
		int nStop);

	//�����豸����
	friend int  RecvDevData(char *DevID, char *buff, int  len);
protected:
	map <string, CComDev*>m_devMap;

	// �����豸����
	int ControlDevSend(char *Dev/*, char *buff, int size*/);


public:
	// ���豸��ӷ�����Ϣ
	int SetSendMsg();
	// �ӷ����б��ȡ������Ϣ
	int GetSendMsg();
};

