#include "UnitsManager.h"

//������
pthread_mutex_t g_unitsSendData_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_units_mutex = PTHREAD_MUTEX_INITIALIZER;

CUnitsManager *g_pComManager = NULL;
int  RemindFun(char *pId)
{
	g_pComManager->ControlDevSend(pId);
	return TRUE;
}
CUnitsManager::CUnitsManager()
{
	g_pComManager = this;
	m_AlarmClock.Start(RemindFun);
}


CUnitsManager::~CUnitsManager()
{
	m_AlarmClock.Stop();
}


int  FeedBackFun(int nStyle, void *object)
{
	printf("FeedBack Style:%d\n", nStyle);
	switch (nStyle)
	{
		case FEEDBACK_SJREQ:  //��������
		{
			printf("SJREQ\n");

			g_pComManager->SetDevSendMsg(*((int *)object));
			g_pComManager->ControlDevSend(*((int*)object));
						
		}
		break;
		case FEEDBACK_ICXX:	//IC��Ϣ
		{
			//DWORD LocalID;
			//char FrameNum;				//����
			////��0֡�����õ��ı���
			//DWORD BroadcastID;			//ͨ��ID
			//char MachineCharacter;		//�û�������
			//unsigned short ServeFreq;			//����Ƶ��
			//unsigned short CommLen;
			//char CommuLeave;			//ͨ�ż���
			//char EncryptFlag;			//���ܱ�־
			//unsigned short SubordinateNum;		//�¼��û�����
			////��N֡�����õ��ı���
			//char CurrFrameSubNum;		//��֡�����û���
			//DWORD lpSubUserAddrs[100];	//ָ���¼��û���ַ��ָ��
			CardInfoRead *pIcInfo = (CardInfoRead *)object;
			//ShowMachineMsg(m_nComID, m_stCardInfo.LocalID, m_stCardInfo.ServeFreq, m_dwCommReqLen, m_nBroadCastID);
			//�޸Ķ�ʱ����Ƶ��
			g_pComManager->m_AlarmClock.ModifTimer(pIcInfo->cDev, pIcInfo->ServeFreq);
			printf("LocalID:%ld  ServeFreq:%d\n", pIcInfo->LocalID, pIcInfo->ServeFreq);
		}
			break;
		case FEEDBACK_ZJXX:	//�Լ���Ϣ
		{
			//DWORD LocalID;
			//char ICCardState;		// IC��״̬
			//char HardState;		// Ӳ��״̬
			//char Batterystate;		// ��ص���
			//char InState;			// ��վ״̬
			//char PowerState[6];	// ����״��
			SelfCheckInfo *pCheck = (SelfCheckInfo *)object;
			printf("ICCardState:%d  HardState:%d  Batterystate:%d InState:%d  PowerState:%s\n",
				pCheck->ICCardState, pCheck->HardState, pCheck->Batterystate, pCheck->InState, pCheck->PowerState);
		}
			break;
		case FEEDBACK_FKXX:	//������Ϣ
		{
			printf("FKXX\n");

			FeedbackInfo *pFeedBack = (FeedbackInfo*)object;
			//char cMsg[100] = { 0 };
			char cResult = pFeedBack->FeedResult;
	
			if (cResult == 0)
			{
				printf("BDTX SUCCESS!\n");
			}
			else
			{
				printf("BDTX ERROR!\n");

			}
			//switch (cResult)
			//{
			//	case 0:
			//	{
			//		if (memcmp(pFeedBack->Reserve, "TXSQ", 4) == 0)
			//		{
			//			sprintf(cMsg, "TX SUCCESS\n");
			//			return TRUE;
			//		}
			//	}
			//		break;
			//	case 1:
			//	{
			//		//strMsg.Format("������Ϣ: %s ʧ��", pResult);
			//	}
			//		break;
			//	case 2:
			//	{
			//		//sprintf(strMsg.c_str, "������Ϣ: %s ʧ��");

			//		//strMsg.Format("������Ϣ:�ź�δ������δ���䣡");
			//	}
			//		break;
			//	case 3:
			//	{
			//		//strMsg.Format("������Ϣ:�������㣬�������ƣ�");
			//	}
			//		break;
			//	case 4:
			//	{
			//		//strMsg.Format("������Ϣ:����Ƶ��δ������ȴ�%d�룡",
			//		//pResult[0] * 1000 + pResult[1] * 100 + pResult[2] * 10 + pResult[3]);
			//	}
			//		break;
			//	case 5:
			//	{
			//		//strMsg.Format("������Ϣ:�ӽ��ܴ���");
			//	}
			//		break;
			//	case 6:
			//	{
			//		//strMsg.Format("������Ϣ:CRC����%s", pResult);
			//	}
			//		break;
			//	case 7:
			//	{
			//		//strMsg.Format("������Ϣ:ϵͳ���ƣ�");
			//	}
			//		break;
			//	case 8:
			//	{
			//		//strMsg.Format("������Ϣ:���߾�Ĭ��");
			//	}
			//		break;
			//	case 9:
			//	{
			//		//strMsg.Format("���ʹ���ʧ�ܣ�");
			//	}
			//		break;
			//}
			g_pComManager->business.SetFeedResData(*pFeedBack);
		}

		
	}
	return TRUE;
}
// ����豸
int CUnitsManager::AddComDev(char *Dev,
	int nSpeed,
	int nBits,
	char nEvent,
	int nStop)
{
	CComUnit *comDevPt = new CComUnit;


	if (comDevPt->Start(Dev, nSpeed, nBits, nEvent, nStop,FeedBackFun) != TRUE)
	{
		return FALSE;
	}


	pthread_mutex_lock(&g_unitsSendData_mutex);

	m_devMap[Dev] = comDevPt;
	m_AlarmClock.AddATimer(Dev, 10);

	pthread_mutex_unlock(&g_unitsSendData_mutex);

	//�Լ�
	SendICJC(Dev);
	SendXTZJ(Dev, 5);

	printf("Add Dev %s Success\n", Dev);
	return TRUE;
}


// ɾ���豸
int CUnitsManager::DelComDev(char *Dev)
{
	pthread_mutex_lock(&g_unitsSendData_mutex);

	map<string, CComUnit*>::iterator it = m_devMap.find(Dev);
	if (it != m_devMap.end())
	{
		CComUnit *comDevPt = it->second;
		char cDevID[20];
		bzero(cDevID, sizeof(cDevID));
		strcpy(cDevID, it->first.c_str());
		m_AlarmClock.DelATimer(cDevID);

		comDevPt->Stop();
		delete comDevPt;
		m_devMap.erase(it);
	}
	pthread_mutex_unlock(&g_unitsSendData_mutex);

	printf("Del Dev %s Success\n", Dev);

	return TRUE;
}


// �޸��豸
int CUnitsManager::ModifComDev(char *Dev,
	int nSpeed,
	int nBits,
	char nEvent,
	int nStop)
{
	if (DelComDev(Dev) == FALSE)
	{
		return FALSE;
	}
	if (AddComDev(Dev, nSpeed, nBits, nEvent, nStop) == FALSE)
	{
		return FALSE;
	}
	printf("modif Dev %s Success\n", Dev);

	return TRUE;
}


// �����豸����
int CUnitsManager::ControlDevSend(char *Dev/*, char *buff, int size*/)
{

	pthread_mutex_lock(&g_unitsSendData_mutex);

	map<string, CComUnit*>::iterator it = m_devMap.find(Dev);
	if (it != m_devMap.end())
	{
		CComUnit *comDevPt = it->second;
	
		//CommReq Data;
		//Data.dwFrameDataLen = 12;
		//Data.pFrameData = { 0x24, 0x49, 0x43, 0x4A, 0x43, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x2B };
		//comDevPt->SetSendMsg(Data);
		comDevPt->ProcessSendMsg();
	}
	pthread_mutex_unlock(&g_unitsSendData_mutex);

	return 0;
}

int CUnitsManager::ControlDevSend(unsigned long nLocalId)
{

	CComUnit *comDevPt = NULL;
	if (g_pComManager->GetComDevFromLocalId(&comDevPt, nLocalId) != TRUE)
	{
		return FALSE;
	}
	comDevPt->ProcessSendMsg();

	return 0;
}
// ���豸��ӷ�����Ϣ
int CUnitsManager::SetDevSendMsg(unsigned long nLocalId, tagBdReq *pData)
{
	CComUnit *comDevPt = NULL;
	if (g_pComManager->GetComDevFromLocalId(&comDevPt, nLocalId) != TRUE)
	{
		return FALSE;
	}

	tagBdReq DataTemp;
	if (pData == NULL)
	{
		pthread_mutex_lock(&g_unitsSendData_mutex);

		DataTemp = m_dataList.front();
		m_dataList.pop_front();
		pthread_mutex_unlock(&g_unitsSendData_mutex);
	}
	else
	{
		memcpy(&DataTemp, pData, sizeof(DataTemp));
	}
	//����������Ϣ���
	tagFrameData FrameData;
	m_parse.SendToBd_TXSQ(DataTemp, FrameData.pFrameData, FrameData.dwFrameDataLen);
	FrameData.dwSerialID = DataTemp.dwSerialID;

	if (comDevPt->SetSendMsg(FrameData) != TRUE)
	{
		return FALSE;
	}
	return TRUE;
}

//���÷�����Ϣ
int CUnitsManager::SetSendMsg(tagBdReq *pData)
{

	pthread_mutex_lock(&g_unitsSendData_mutex);

	//ָ��ָ�ӻ���ͨ��
	if (pData->DestAddress == 0 && pData->SourceAddress != 0)
	{
		//ָ���豸��������
		if (SetDevSendMsg(pData->SourceAddress, pData) != TRUE)
		{
			m_dataList.push_back(*pData);
		}

	}
	//����ָ�ӻ�����
	else if (pData->DestAddress != 0)
	{
		m_dataList.push_back(*pData);

	}
	else 
	{
		printf("BDSF Msg Error\n");
	}

	pthread_mutex_unlock(&g_unitsSendData_mutex);

	return TRUE;
}
// ��ȡ�豸
int CUnitsManager::GetComDevFromDevId(CComUnit** comDevPt, char *Dev)
{
	pthread_mutex_lock(&g_unitsSendData_mutex);

	map<string, CComUnit*>::iterator it = m_devMap.find(Dev);
	if (it != m_devMap.end())
	{
		*comDevPt = it->second;
		pthread_mutex_unlock(&g_unitsSendData_mutex);

		return TRUE;
	}
	pthread_mutex_unlock(&g_unitsSendData_mutex);

	return FALSE;
}
int CUnitsManager::GetComDevFromLocalId(CComUnit** comDevPt, unsigned long nLocalId)
{
	pthread_mutex_lock(&g_unitsSendData_mutex);

	map<string, CComUnit*>::iterator it = m_devMap.begin();
	while (it != m_devMap.end())
	{
		*comDevPt = it->second;
		if ((*comDevPt)->m_nLocalID == nLocalId)
		{
			pthread_mutex_unlock(&g_unitsSendData_mutex);

			return TRUE;
		}
		
		it++;
	}
	pthread_mutex_unlock(&g_unitsSendData_mutex);

	return FALSE;
}


// IC���Լ�
int CUnitsManager::SendICJC(char *Dev)
{
	tagFrameData Data;
	Data.dwFrameDataLen = m_parse.SendToBd_ICJC(0, 0, Data.pFrameData);

	CComUnit *comDevPt = NULL;
	if (g_pComManager->GetComDevFromDevId(&comDevPt, Dev) != TRUE)
	{
		return FALSE;
	}
	if (comDevPt->SetSendMsg(Data) != TRUE)
	{
		return FALSE;
	}
	comDevPt->ProcessSendMsg();

	return TRUE;

}


// ϵͳ�Լ�
int CUnitsManager::SendXTZJ(char *Dev,int nZJPD)
{
	tagFrameData Data;
	Data.dwFrameDataLen = m_parse.SendToBd_XTZJ(0, nZJPD, Data.pFrameData);

	CComUnit *comDevPt = NULL;
	if (g_pComManager->GetComDevFromDevId(&comDevPt, Dev) != TRUE)
	{
		return FALSE;
	}
	if (comDevPt->SetSendMsg(Data) != TRUE)
	{
		return FALSE;
	}
	comDevPt->ProcessSendMsg();
	return TRUE;
}
