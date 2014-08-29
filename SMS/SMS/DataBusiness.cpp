#include "DataBusiness.h"

//互斥锁
pthread_mutex_t g_busiessData_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_FeedData_mutex = PTHREAD_MUTEX_INITIALIZER;
extern CUnitsManager *g_pComManager;
void *threadBusiess(void *arg);
extern CDataTransfer *g_pDatatransfer = NULL;

void GetCommInfoMsg(CommInfo& stCommInfo, string& strCommInfo)
{
	char ctemp[256] = { 0 };
	sprintf(ctemp, "发送时间:[%d]时[%d]分[%d]秒  ", stCommInfo.CommHour, stCommInfo.CommMin, stCommInfo.CommSecond);
	string sTemp(ctemp);

	strCommInfo += sTemp;
	switch (stCommInfo.ifBCD)
	{
	case 0:
		strCommInfo += "[汉字";
		break;
	case 1:
		strCommInfo += "[代码";
		break;
	case 2:
		strCommInfo += "[混发";
		break;
	}

	if (stCommInfo.ifSecret)
		strCommInfo += "__加密";
	else
		strCommInfo += "__非密";

	if (stCommInfo.ifUrgent)
		strCommInfo += "__普通]";
	else
		strCommInfo += "__加急]";

	
	sprintf(ctemp, "  电文内容:[%d]字节 [%s] ", stCommInfo.CommLenByte, stCommInfo.CommBuff);
	string sTemp2(ctemp);
	strCommInfo += sTemp2;
}

CDataBusiness::CDataBusiness()
{
	pthread_create(&m_DataBusinessPt, NULL, threadBusiess, this);

}


CDataBusiness::~CDataBusiness()
{
}

void *threadBusiess(void *arg)
{
	pthread_detach(pthread_self());

	int state, oldstate;

	state = PTHREAD_CANCEL_DEFERRED;
	pthread_setcancelstate(state, &oldstate);

	CDataBusiness *me = (CDataBusiness *)arg;
	while (true)
	{
		if (me->ProcessBusiess() != TRUE)
		{
			break;
		}
		pthread_testcancel();

	}
	return NULL;
}
int CDataBusiness::ProcessBusiess()
{
	string sMsg = "";
	//1、解析BD通信信息
	list<CommInfo *> dataList;

	//獲取數據
	pthread_mutex_lock(&g_busiessData_mutex);
	dataList.insert(dataList.begin(), m_dataList.begin(), m_dataList.end());
	m_dataList.clear();
	pthread_mutex_unlock(&g_busiessData_mutex);

	char pExplainData[1000];
	unsigned long dwExplLen;

	while (!dataList.empty())
	{
		CommInfo * p = dataList.front();
		if (parseData.ExplainData_UDP(p->pFrameData, pExplainData, dwExplLen))
		{
			CommInfo stCommInfo;
			stCommInfo = *(CommInfo*)pExplainData;
			//存入数据库(待加)

			//解析存入控制模块
			g_pComManager->SetSendMsg(stCommInfo);

			//显示信息
			GetCommInfoMsg(stCommInfo, sMsg);
			printf("%s\n", sMsg.c_str());
		}

		delete p->pFrameData;
		dataList.pop_front();
	}



	//2、打包通信反馈信息
	//獲取數據
	list<FeedbackInfo> resdataList;

	pthread_mutex_lock(&g_FeedData_mutex);
	resdataList.insert(resdataList.begin(), m_ResdataList.begin(), m_ResdataList.end());
	m_ResdataList.clear();
	pthread_mutex_unlock(&g_FeedData_mutex);

	char cSendBuff[1024];
	bzero(cSendBuff, sizeof(cSendBuff));
	unsigned long nSendLength;
	while (!resdataList.empty())
	{
		CommInfo  FeedbackInfo = resdataList.front();
		parseData.SendToDS_FKXX(FeedbackInfo, cSendBuff,nSendLength);
		if (g_pDatatransfer != NULL)
		{
			g_pDatatransfer->SendData(cSendBuff, nSendLength);
		}
	}
	return TRUE;
}
// 设置业务数据
int CDataBusiness::SetBusiessData(CommInfo *pCommInfo)
{
	pthread_mutex_lock(&g_busiessData_mutex);
	m_dataList.push_back(pCommInfo);
	pthread_mutex_unlock(&g_busiessData_mutex);

	return 0;
}

//设置反馈
int CDataBusiness::SetFeedResData(FeedbackInfo &stBackInfo)
{
	pthread_mutex_lock(&g_FeedData_mutex);
	m_ResdataList.push_back(stBackInfo);
	pthread_mutex_unlock(&g_FeedData_mutex);

	return 0;
}
