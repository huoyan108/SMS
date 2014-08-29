#pragma once
#include <list>
#include <string.h>
#include "UnitsManager.h"
using namespace std;
#include "PareData.h"
class CDataBusiness
{
public:
	CDataBusiness();
	~CDataBusiness();
	// ����ҵ������
	int SetBusiessData(CommInfo *pCommInfo);
	//���û�ִ�������
	int SetFeedResData(FeedbackInfo &stBackInfo);
	//���������̣߳�����������
	pthread_t m_DataBusinessPt;

	int ProcessBusiess();

	CPareData parseData;
protected:
	//���ݴ����б�
	list<CommInfo *> m_dataList;
	//ͨ�ŷ����б�
	list<FeedbackInfo> m_ResdataList;

};

