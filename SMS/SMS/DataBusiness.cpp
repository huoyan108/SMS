#include "DataBusiness.h"
#include "IniOper.h"

//������
pthread_mutex_t g_busiessData_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_FeedData_mutex = PTHREAD_MUTEX_INITIALIZER;
extern CUnitsManager *g_pComManager;
void *threadBusiess(void *arg);
//extern CDataTransfer *g_pDatatransfer = NULL;
CDataBusiness * g_pDataBusiness = NULL;

int NotifFun(char * sendName, void *pReq)
{
	g_pDataBusiness->SetBusiessData((BdfsMsg *)pReq);
	return 1;
}

CDataBusiness::CDataBusiness()
{
	g_pDataBusiness = this;
	pthread_create(&m_DataBusinessPt, NULL, threadBusiess, this);

	//���ͨ�Ŷ˿ڲ���
	char cPath[1000];
	bzero(cPath, sizeof(cPath));

	CIniOper ini;

	if (ini.GetSoftPath(cPath, sizeof(cPath)) != 0)
	{
		exit(0);
	}
	strcat(cPath, "ini.lua");
	int nRequestPort;
	int nRespondPort;
	if (ini.load(cPath, &nRequestPort, &nRespondPort) != 0)
	{
		exit(0);
	}
	
	//������תͨ�ŷ���
	m_treansfer.StartZmq(nRequestPort, nRespondPort, NotifFun);
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
		sleep(1);
	}
	return NULL;
}
int CDataBusiness::ProcessBusiess()
{
	//string sMsg = "";
	////1������BDͨ����Ϣ
	//list<CommReq *> dataList;

	////�@ȡ����
	//pthread_mutex_lock(&g_busiessData_mutex);
	//dataList.insert(dataList.begin(), m_dataList.begin(), m_dataList.end());
	//m_dataList.clear();
	//pthread_mutex_unlock(&g_busiessData_mutex);

	//char pExplainData[1000];
	//DWORD dwExplLen;

	//while (!dataList.empty())
	//{
	//	CommReq * p = dataList.front();
	//
	//		//�������ݿ�(����)

	//		//�����������ģ��
	//		g_pComManager->SetSendMsg(stCommInfo);

	//		//��ʾ��Ϣ


	//	delete p;
	//	dataList.pop_front();
	//}



	//2�����ͨ�ŷ�����Ϣ
	//�@ȡ����
	list<FeedbackInfo> resdataList;
	pthread_mutex_lock(&g_FeedData_mutex);
	resdataList.insert(resdataList.begin(), m_ResdataList.begin(), m_ResdataList.end());
	m_ResdataList.clear();
	pthread_mutex_unlock(&g_FeedData_mutex);

	char cSendBuff[1024];
	bzero(cSendBuff, sizeof(cSendBuff));
	DWORD nSendLength;
	while (!resdataList.empty())
	{
		printf("send Feedback\n");

		FeedbackInfo  FeedbackInfo = resdataList.front();
		//parseData.SendToDS_FKXX(FeedbackInfo, cSendBuff,nSendLength);
		//m_treansfer.SendData(cSendBuff, nSendLength);
	}
	return TRUE;
}
// ����ҵ������
int CDataBusiness::SetBusiessData(BdfsMsg *pCommReq)
{
	//pthread_mutex_lock(&g_busiessData_mutex);
	//m_dataList.push_back(pCommReq);
	//pthread_mutex_unlock(&g_busiessData_mutex);
	//�������ݿ�(����)

	//�����������ģ��
	g_pComManager->SetSendMsg(pCommReq);

	//���÷��������յ�BDFS����

	return 0;
}

//���÷���
int CDataBusiness::SetFeedResData(FeedbackInfo &stBackInfo)
{
	pthread_mutex_lock(&g_FeedData_mutex);
	m_ResdataList.push_back(stBackInfo);
	pthread_mutex_unlock(&g_FeedData_mutex);

	return 0;
}
