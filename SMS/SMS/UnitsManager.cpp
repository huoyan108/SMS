#include "UnitsManager.h"

int  RemindFun(char *pId)
{
	return TRUE;
}
CUnitsManager *g_pComManager = NULL;
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

			CComUnit *comDevPt = NULL;
			if (g_pComManager->GetComDev(&comDevPt, (char *)object) == TRUE)
			{
				tagFrameData Data;
				Data.dwFrameDataLen = 12;
				Data.pFrameData = { 0x24, 0x49, 0x43, 0x4A, 0x43, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x2B };
				comDevPt->SetSendMsg(Data);

				g_pComManager->ControlDevSend((char *)object);
				sleep(10);
			}
			
		}
		break;
		case FEEDBACK_ICXX:	//IC��Ϣ
		{
			//unsigned long LocalID;
			//char FrameNum;				//����
			////��0֡�����õ��ı���
			//unsigned long BroadcastID;			//ͨ��ID
			//char MachineCharacter;		//�û�������
			//unsigned short ServeFreq;			//����Ƶ��
			//unsigned short CommLen;
			//char CommuLeave;			//ͨ�ż���
			//char EncryptFlag;			//���ܱ�־
			//unsigned short SubordinateNum;		//�¼��û�����
			////��N֡�����õ��ı���
			//char CurrFrameSubNum;		//��֡�����û���
			//unsigned long lpSubUserAddrs[100];	//ָ���¼��û���ַ��ָ��
			CardInfoRead *pIcInfo = (CardInfoRead *)object;
			//ShowMachineMsg(m_nComID, m_stCardInfo.LocalID, m_stCardInfo.ServeFreq, m_dwCommReqLen, m_nBroadCastID);
			printf("LocalID:%ld  ServeFreq:%ld\n", pIcInfo->LocalID, pIcInfo->ServeFreq);
		}
			break;
		case FEEDBACK_ZJXX:	//�Լ���Ϣ
		{
			//unsigned long LocalID;
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
			char cMsg[100] = { 0 };
			char cResult = pFeedBack->FeedResult;
	
			switch (cResult)
			{
				case 0:
				{
					if (memcmp(pFeedBack->Reserve, "TXSQ", 4) == 0)
					{
						sprintf(cMsg, "SEND SUCCESS\n");
						return TRUE;
					}
				}
					break;
				case 1:
				{
					//strMsg.Format("������Ϣ: %s ʧ��", pResult);
				}
					break;
				case 2:
				{
					//sprintf(strMsg.c_str, "������Ϣ: %s ʧ��");

					//strMsg.Format("������Ϣ:�ź�δ������δ���䣡");
				}
					break;
				case 3:
				{
					//strMsg.Format("������Ϣ:�������㣬�������ƣ�");
				}
					break;
				case 4:
				{
					//strMsg.Format("������Ϣ:����Ƶ��δ������ȴ�%d�룡",
					//pResult[0] * 1000 + pResult[1] * 100 + pResult[2] * 10 + pResult[3]);
				}
					break;
				case 5:
				{
					//strMsg.Format("������Ϣ:�ӽ��ܴ���");
				}
					break;
				case 6:
				{
					//strMsg.Format("������Ϣ:CRC����%s", pResult);
				}
					break;
				case 7:
				{
					//strMsg.Format("������Ϣ:ϵͳ���ƣ�");
				}
					break;
				case 8:
				{
					//strMsg.Format("������Ϣ:���߾�Ĭ��");
				}
					break;
			}
		}

		
	}
	//g_pComManager->m_devMap["ad"];
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

	m_devMap[Dev] = comDevPt;
	m_AlarmClock.AddATimer(Dev, 500);
	return TRUE;
}


// ɾ���豸
int CUnitsManager::DelComDev(char *Dev)
{
	map<string, CComUnit*>::iterator it = m_devMap.find(Dev);
	if (it != m_devMap.end())
	{
		CComUnit *comDevPt = it->second;
		//m_AlarmClock.DelATimer(it->first);

		comDevPt->Stop();
		m_devMap.erase(it);
	}
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
	return TRUE;
}


// �����豸����
int CUnitsManager::ControlDevSend(char *Dev/*, char *buff, int size*/)
{
	map<string, CComUnit*>::iterator it = m_devMap.find(Dev);
	if (it != m_devMap.end())
	{
		CComUnit *comDevPt = it->second;
	
		comDevPt->ProcessSendMsg();
	}
	return 0;
}


// ���豸��ӷ�����Ϣ
int CUnitsManager::SetSendMsg()
{
	return 0;
}


// �ӷ����б��ȡ������Ϣ
int CUnitsManager::GetSendMsg()
{
	return 0;
}


// ��ȡ�豸
int CUnitsManager::GetComDev(CComUnit** comDevPt, char *Dev)
{
	map<string, CComUnit*>::iterator it = m_devMap.find(Dev);
	if (it != m_devMap.end())
	{
		*comDevPt = it->second;
		return TRUE;
	}
	return FALSE;
}
