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
	int SetBusiessData(tagFrameData *pTagFrameData);
	//���û�ִ�������
	int SetFeedResData();
	//���������̣߳�����������
	pthread_t m_DataBusinessPt;

	int ProcessBusiess();

	CPareData parseData;
protected:
	//���ݴ����б�
	list<tagFrameData *> m_dataList;
	//ͨ�ŷ����б�
	list<tagFrameData *> m_ResdataList;

};

