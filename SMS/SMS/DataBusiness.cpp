#include "DataBusiness.h"

//������
pthread_mutex_t g_busiessData_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_FeedData_mutex = PTHREAD_MUTEX_INITIALIZER;
extern CUnitsManager *g_pComManager;
void *threadBusiess(void *arg);

void GetCommInfoMsg(CommInfo& stCommInfo, string& strCommInfo)
{
	strCommInfo.Format("����ʱ��:[%d]ʱ[%d]��[%d]��  ", stCommInfo.CommHour, stCommInfo.CommMin, stCommInfo.CommSecond);

	switch (stCommInfo.ifBCD)
	{
	case 0:
		strCommInfo += "[����";
		break;
	case 1:
		strCommInfo += "[����";
		break;
	case 2:
		strCommInfo += "[�췢";
		break;
	}

	if (stCommInfo.ifSecret)
		strCommInfo += "__����";
	else
		strCommInfo += "__����";

	if (stCommInfo.ifUrgent)
		strCommInfo += "__��ͨ]";
	else
		strCommInfo += "__�Ӽ�]";

	CString strMsg;
	strMsg.Format("  ��������:[%d]�ֽ� [%s]", stCommInfo.CommLenByte, stCommInfo.CommBuff);

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
	//1������BDͨ����Ϣ
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
				//�������ݿ�(����)

				//�����������ģ��
				g_pComManager->SetSendMsg(stCommInfo);

				//��ʾ��Ϣ
				GetCommInfoMsg(stCommInfo, sMsg);
				printf("%s\n", sMsg);
			}

		}
	}


	//2�����ͨ�ŷ�����Ϣ

	return TRUE;
}
// ����ҵ������
int CDataBusiness::SetBusiessData(tagFrameData *pFrameData)
{
	pthread_mutex_lock(&g_busiess_mutex);
	m_dataList.push_back(pTagFrameData);
	pthread_mutex_unlock(&g_busiess_mutex);



	return 0;
}

//���÷���
int CDataBusiness::SetFeedResData()
{
	pthread_mutex_lock(&g_FeedData_mutex);
	//m_ResdataList
	pthread_mutex_unlock(&g_FeedData_mutex);

	return 0;
}
