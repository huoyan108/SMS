#pragma once
#include <map>
#include <string>
using namespace std;
#include "ComUnit.h"
#include "AlarmClock.h"

class CUnitsManager
{
public:
	CUnitsManager();
	~CUnitsManager();
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
	//Ӳ����Ԫ����
	map <string, CComUnit*>m_devMap;

	//�������ݴ���
	friend int  FeedBackFun(int nStyle,void *object);

	//��ʱ����
	friend int  RemindFun(char *pId);

	CAlarmClock m_AlarmClock;

public:
	// ���豸��ӷ�����Ϣ
	int SetSendMsg();
	// �ӷ����б��ȡ������Ϣ
	int GetSendMsg();

	// �����豸����
	int ControlDevSend(char *Dev/*, char *buff, int size*/);

	// ��ȡ�豸
	int GetComDev(CComUnit** comDevPt, char *Dev);
};

