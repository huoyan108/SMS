#pragma once
#include <map>
#include <string>
#include <list>
#include "ComUnit.h"
#include "AlarmClock.h"
#include "PareData.h"
using namespace std;
#include "DataBusiness.h"
#include "IniOper.h"
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
	int SetSendMsg(tagBdReq *pData);

	// �����豸����
	int ControlDevSend(char *Dev/*, char *buff, int size*/);
	int ControlDevSend(unsigned long nLocalId);

	//�����豸����
	friend int  RecvDevData(char *DevID, char *buff, int  len);
protected:
	CDataBusiness business;

	//�豸��Ԫ����
	map <string, CComUnit*>m_devMap;

	//�������ݴ���
	friend int  FeedBackFun(int nStyle,void *object);

	//��ʱ����
	friend int  RemindFun(char *pId);

	CAlarmClock m_AlarmClock;

	CPareData m_parse;

	//���ݷ����б�
	list<tagBdReq> m_dataList;

	// ���豸��Ԫ��ӷ�����Ϣ,���pDataΪ�գ����б��ȡ
	//int SetDevSendMsg(char *DevID, tagBdReq *pData = NULL);
	int SetDevSendMsg(unsigned long nLocalId, tagBdReq *pData = NULL);

	// ��ȡ�豸
	int GetComDevFromDevId(CComUnit** comDevPt, char *Dev);
	int GetComDevFromLocalId(CComUnit** comDevPt, unsigned long nLocalId);

public:
	// IC���Լ�
	int SendICJC(char *Dev);
	// ϵͳ�Լ�
	int SendXTZJ(char *Dev, int nZJPD);
};

