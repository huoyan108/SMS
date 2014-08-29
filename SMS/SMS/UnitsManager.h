#pragma once
#include <map>
#include <string>
#include <list>
#include "ComUnit.h"
#include "AlarmClock.h"
using namespace std;

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

	//���÷�����Ϣ
	int SetSendMsg(CommInfo &Data);

	// �����豸����
	int ControlDevSend(char *Dev/*, char *buff, int size*/);

	//�����豸����
	friend int  RecvDevData(char *DevID, char *buff, int  len);
protected:
	//�豸��Ԫ����
	map <string, CComUnit*>m_devMap;

	//�������ݴ���
	friend int  FeedBackFun(int nStyle,void *object);

	//��ʱ����
	friend int  RemindFun(char *pId);

	CAlarmClock m_AlarmClock;

	//���ݷ����б�
	list<CommInfo> m_dataList;

	// ���豸��Ԫ��ӷ�����Ϣ
	int SetDevSendMsg(char *DevID, CommInfo *pData = NULL);

	// ��ȡ�豸
	int GetComDev(CComUnit** comDevPt, char *Dev);

};

