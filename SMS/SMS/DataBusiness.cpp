#include "DataBusiness.h"

//互斥锁
pthread_mutex_t g_busiessData_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_FeedData_mutex = PTHREAD_MUTEX_INITIALIZER;
extern CUnitsManager *g_pComManager;
void *threadBusiess(void *arg);

void GetCommInfoMsg(CommInfo& stCommInfo, string& strCommInfo)
{
	strCommInfo.Format("发送时间:[%d]时[%d]分[%d]秒  ", stCommInfo.CommHour, stCommInfo.CommMin, stCommInfo.CommSecond);

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

	CString strMsg;
	strMsg.Format("  电文内容:[%d]字节 [%s]", stCommInfo.CommLenByte, stCommInfo.CommBuff);

	strCommInfo += strMsg;
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
	list<tagFrameData *> dataList;

	pthread_mutex_lock(&g_busiess_mutex);
	dataList.insert(dataList.begin(), m_dataList.begin(), m_dataList.end());
	m_dataList.clear();
	pthread_mutex_unlock(&g_busiess_mutex);

	char pExplainData[1000];
	unsigned long dwExplLen;
	if (dataList.size() > 0)
	{
		for (list<tagFrameData *>::iterator it = m_dataList.begin();  it != m_dataList.end(); it++)
		{
			if (parseData.ExplainData_UDP(it, pExplainData, dwExplLen))
			{
				CommInfo stCommInfo;
				stCommInfo = *(CommInfo*)pExplainData;
				//存入数据库(待加)

				//解析存入控制模块
				g_pComManager->SetSendMsg(stCommInfo);

				//显示信息
				GetCommInfoMsg(stCommInfo, sMsg);
				printf("%s\n", sMsg);
			}

		}
	}


	//2、打包通信反馈信息

	return TRUE;
}
// 设置业务数据
int CDataBusiness::SetBusiessData(tagFrameData *pFrameData)
{
	pthread_mutex_lock(&g_busiess_mutex);
	m_dataList.push_back(pTagFrameData);
	pthread_mutex_unlock(&g_busiess_mutex);



	return 0;
}

//设置反馈
int CDataBusiness::SetFeedResData()
{
	pthread_mutex_lock(&g_FeedData_mutex);
	//m_ResdataList
	pthread_mutex_unlock(&g_FeedData_mutex);

	return 0;
}
