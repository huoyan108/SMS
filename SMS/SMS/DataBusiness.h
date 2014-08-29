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
	// 设置业务数据
	int SetBusiessData(CommInfo *pCommInfo);
	//设置回执结果数据
	int SetFeedResData(FeedbackInfo &stBackInfo);
	//开启处理线程，打包解包推送
	pthread_t m_DataBusinessPt;

	int ProcessBusiess();

	CPareData parseData;
protected:
	//数据处理列表
	list<CommInfo *> m_dataList;
	//通信反馈列表
	list<FeedbackInfo> m_ResdataList;

};

