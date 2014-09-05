#pragma once
#include <list>
#include <string.h>
#include <time.h>

using namespace std;
#include "PareData.h"
#include "DataTransfer.h"
#include "PGDatabase.h"

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
	PGDatabase m_db;

	friend int NotifFun(char * sendName, void *pReq);

	CDataTransfer m_treansfer;

	// ����ҵ������
	int SetBusiessData(tagBdReq *pCommReq);

	//�������
	CPareData parseData;

	//���ݴ����б�
	//list<CommReq *> m_dataList;
	//ͨ�ŷ����б�
	list<FeedbackInfo> m_ResdataList;

};

