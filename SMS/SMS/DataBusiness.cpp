#include "DataBusiness.h"
#include "IniOper.h"
#include "UnitsManager.h"

//������
pthread_mutex_t g_busiessData_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_FeedData_mutex = PTHREAD_MUTEX_INITIALIZER;
extern CUnitsManager *g_pComManager;
void *threadBusiess(void *arg);
//extern CDataTransfer *g_pDatatransfer = NULL;
CDataBusiness * g_pDataBusiness = NULL;

int NotifFun(char * sendName, void *pReq)
{
	g_pDataBusiness->SetBusiessData((tagBdReq *)pReq);
	return 1;
}

CDataBusiness::CDataBusiness()
{
	g_pDataBusiness = this;
	pthread_create(&m_DataBusinessPt, NULL, threadBusiess, this);



	CIniOper ini;

	//��ȡ·��
	char cPath[1000];
	bzero(cPath, sizeof(cPath));
	if (ini.GetSoftPath(cPath, sizeof(cPath)) != 0)
	{
		exit(0);
	}
	strcat(cPath, "ini.lua");

	//��ȡ����
	int nRequestPort;
	int nRespondPort;
	char cSoftName[100];
	bzero(cSoftName, sizeof(cSoftName));

	if (ini.load(cPath, &nRequestPort, &nRespondPort, cSoftName) != 0)
	{
		printf("read ini error!\n");
		exit(0);
	}
	
	//������תͨ�ŷ���
	m_treansfer.StartZmq(nRequestPort, nRespondPort, cSoftName,NotifFun);


	//��ȡ���ݿ����Ӳ���
	PGConnInfo conn;
	char dbhost[100];
	char dbport[100];
	char dbname[100];
	char dbuser[100];
	char dbpwd[100];
	if (ini.load(cPath, dbhost,dbport,dbname,dbuser,dbpwd) != 0)
	{
		printf("read ini error!\n");

		exit(0);
	}
	conn.pghost = (char*)dbhost;
	conn.pgport = (char*)dbport;
	conn.dbName = (char*)dbname;
	conn.login = (char*)dbuser;
	conn.passwd = (char*)dbpwd;

	//�������ݿ�
	if (0 != m_db.Connect(conn))
	{
		printf("connect Db error!\n");

		exit(0);
	}
	printf("connect Db OK\n");
}


CDataBusiness::~CDataBusiness()
{
}

void *threadBusiess(void *arg)
{
	pthread_detach(pthread_self());

	//int state, oldstate;

	//state = PTHREAD_CANCEL_DEFERRED;
	//pthread_setcancelstate(state, &oldstate);

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
	DWORD nSendLength = 0;
	while (!resdataList.empty())
	{
		printf("send Feedback\n");

		FeedbackInfo  FeedbackInfo = resdataList.front();

		//�������ݿ�
		struct tm * timeinfo;
		timeinfo = localtime(&FeedbackInfo.sendtime);
		printf("The Send date/time is: %s", asctime(timeinfo));
		char sSql[256];
		bzero(sSql, sizeof(sSql));
		//UPDATE ������ SET ������ = ��ֵ WHERE ������ = ĳֵ
		sprintf(sSql, "update bss_bdfs set send_time=to_date('%s','YYYY-MM-DD H24:MI:SS'),send_times=%d,status=%d",
			asctime(timeinfo),
			FeedbackInfo.sendtimes,
			FeedbackInfo.FeedResult);
		m_db.Exec(sSql);

		/*//���
		parseData.SendToDS_FKXX(FeedbackInfo.dwSerialID,
			FeedbackInfo.FeedResult,
			cSendBuff, 
			nSendLength);
		//����
		m_treansfer.SendData(cSendBuff, nSendLength);*/
		//���
		FsfkMsg fsfkMsg;
		fsfkMsg.set_nserialid(FeedbackInfo.dwSerialID);
		fsfkMsg.set_nres(atoi(FeedbackInfo.FeedResult));

		string sBuff = "";
		fsfkMsg.SerializeToString(&sBuff);
		//����
		m_treansfer.SendData((char*)sBuff.c_str(), sBuff.length());
	}
	return TRUE;
}

// ����ҵ������
int CDataBusiness::SetBusiessData(tagBdReq *pCommReq)
{
	//�������ݿ�
	//��ȡ��ǰʱ��
	time_t currtime;
	struct tm * timeinfo;

	time(&currtime);
	timeinfo = localtime(&currtime);
	printf("The current date/time is: %s", asctime(timeinfo));
	char sSql[256];
	bzero(sSql, sizeof(sSql));

	sprintf(sSql,"insert into bss_bdfs(id,sender,recver,msg_len,msg_data,msg_type,submit_time) values (bss_bdfs_id_seq.nextval,%ld,%ld,%ld,'%s','%d',to_date('%s','YYYY-MM-DD H24:MI:SS'))",
		pCommReq->SourceAddress,
		pCommReq->DestAddress,
		pCommReq->InfoLen,
		pCommReq->InfoBuff,
		pCommReq->nMsgType,
		asctime(timeinfo));
	m_db.Exec(sSql);
	//�����������ģ��
	FeedbackInfo stBackInfo;

	if (g_pComManager->SetSendMsg(pCommReq) == TRUE)
	{
		//���÷������ύ�ɹ�
		stBackInfo.dwSerialID = pCommReq->dwSerialID;
		stBackInfo.FeedResult = 0;

		SetFeedResData(stBackInfo);
	}
	else
	{
		//�ύʧ��
		stBackInfo.dwSerialID = pCommReq->dwSerialID;
		stBackInfo.FeedResult = 3;

		SetFeedResData(stBackInfo);
	}
	

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
