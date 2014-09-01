#pragma once
#include <list>
#include <string.h>
#include "UnitsManager.h"
using namespace std;
#include "PareData.h"
#include "DataTransfer.h"
class CDataBusiness
{
public:
	CDataBusiness();
	~CDataBusiness();
	//���û�ִ�������
	int SetFeedResData(FeedbackInfo &stBackInfo);

	//���������̣߳�����������
	pthread_t m_DataBusinessPt;

	int ProcessBusiess();

protected:
	friend int NotifFun(char * sendName, void *pReq);

	CDataTransfer m_treansfer;

	// ����ҵ������
	int SetBusiessData(CommReq *pCommReq);

	//�������
	CPareData parseData;

	//���ݴ����б�
	list<CommReq *> m_dataList;
	//ͨ�ŷ����б�
	list<FeedbackInfo> m_ResdataList;

};

