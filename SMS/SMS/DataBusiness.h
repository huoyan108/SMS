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
	//设置回执结果数据
	int SetFeedResData(FeedbackInfo &stBackInfo);

	//开启处理线程，打包解包推送
	pthread_t m_DataBusinessPt;

	int ProcessBusiess();

protected:
	PGDatabase m_db;

	friend int NotifFun(char * sendName, void *pReq);

	CDataTransfer m_treansfer;

	// 设置业务数据
	int SetBusiessData(tagBdReq *pCommReq);

	//解析打包
	CPareData parseData;

	//数据处理列表
	//list<CommReq *> m_dataList;
	//通信反馈列表
	list<FeedbackInfo> m_ResdataList;

};

