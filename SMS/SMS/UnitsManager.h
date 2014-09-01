#pragma once
#include <map>
#include <string>
#include <list>
#include "ComUnit.h"
#include "AlarmClock.h"
#include "PareData.h"
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
	int SetSendMsg(CommReq &Data);

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

	CPareData m_parse;

	//���ݷ����б�
	list<CommReq> m_dataList;

	// ���豸��Ԫ��ӷ�����Ϣ,���pDataΪ�գ����б��ȡ
	int SetDevSendMsg(char *DevID, CommReq *pData = NULL);

	// ��ȡ�豸
	int GetComDev(CComUnit** comDevPt, char *Dev);

public:
	// IC���Լ�
	int SendICJC(char *Dev);
	// ϵͳ�Լ�
	int SendXTZJ(char *Dev, int nZJPD);
};

