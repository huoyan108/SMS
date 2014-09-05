#include "ComUnit.h"

int  RecvDevData(void *object,char *DevID, char *buff, int  len)
{
	CComUnit *me = (CComUnit *)object;
	if (me == NULL)
	{
		printf("CComUnit * Error\n");
		return FALSE;
	}

	//��������
	char pEncodeData[1000];
	bzero(pEncodeData, sizeof(pEncodeData));

	int nReportID = me->parseData.ExplainData_BD(buff, len, pEncodeData);
	switch (nReportID)
	{
		case CUNPACK_FKXX:
		{
			FeedbackInfo stBackInfo;
			stBackInfo = *(FeedbackInfo*)pEncodeData;


			//����ɹ�ɾ�����ط����б�(��ȷ��,Ŀǰȷ�Ϸ��;�ɾ��)
			//ʧ�ܵȴ��´η���(��ȷ��)
			map<DWORD, tagFrameData &>::iterator it = me->m_sendMsgList.begin();
			if (it != me->m_sendMsgList.end())
			{
				stBackInfo.dwSerialID = it->first;

				//����ʱ��
				time_t currtime;
				time(&currtime);
				stBackInfo.sendtime = currtime;

				//���ʹ���
				if (stBackInfo.dwSerialID == me->m_dwSerialID)
				{
					stBackInfo.sendtimes = me->m_nSendTimes;
				}
				

				me->m_sendMsgList.erase(it);
			}
		
			//���÷�����Ϣ
			me->SetTXRes(stBackInfo);
			me->m_myFun(FEEDBACK_FKXX, &stBackInfo);
		}
		break;
		case CUNPACK_ICXX:
		{
			CardInfoRead stCardInfo;
			stCardInfo = *(CardInfoRead*)pEncodeData;
			me->SetIcCardMsg(stCardInfo);
		}
		break;
		case CUNPACK_ZJXX:
		{
			SelfCheckInfo stSelfCheck;
			stSelfCheck = *(SelfCheckInfo*)pEncodeData;

			
			me->SetMachineStatus(stSelfCheck);
		}
		break;
		default:
			break;
	}
	return true;
}

CComUnit::CComUnit() :/*m_nSendSeq(0),*/
m_myFun(0),
m_nLocalID(0),
m_dwSerialID(0),
m_nSendTimes(0)
{

}

CComUnit::~CComUnit()
{
}


int CComUnit::Start(char *Dev,
	int nSpeed,
	int nBits,
	char nEvent,
	int nStop,
	FeedBack myFun)
{
	m_comDev.SetParam(this,Dev, nSpeed, nBits, nEvent, nStop, RecvDevData);
	if (m_comDev.Start() != TRUE)
	{
		return FALSE;
	}
	strcat(m_cDev, Dev);
	m_myFun = myFun;
	return TRUE;
}
int CComUnit::Stop()
{
	m_comDev.Stop();
	return TRUE;
}
void CComUnit::SetTXRes(FeedbackInfo &stBackInfo)
{
	printf("FeedbackInfo RECV Notif\n");

}

void CComUnit::SetMachineStatus(SelfCheckInfo& stCheckInfo)
{
	printf("SelfCheckInfo RECV Notif\n");

	m_tLastGetCheckMsg = time(NULL);

	m_stCheckInfo = stCheckInfo;

	//��ʾ
	m_myFun(FEEDBACK_ZJXX, &m_stCheckInfo);

}
void CComUnit::SetIcCardMsg(CardInfoRead& stCardInfo)
{
	printf("CardInfoRead RECV Notif\n");
	m_stCardInfo = stCardInfo;
	m_nLocalID = stCardInfo.LocalID;
	strcat(stCardInfo.cDev, m_cDev);

	switch (m_stCardInfo.CommuLeave)
	{
	case 1:
		m_dwCommReqLen = m_stCardInfo.EncryptFlag ? 140 : 110;
		break;
	case 2:
		m_dwCommReqLen = m_stCardInfo.EncryptFlag ? 360 : 408;
		break;
	case 3:
		m_dwCommReqLen = m_stCardInfo.EncryptFlag ? 580 : 628;
		break;
	case 4:
		m_dwCommReqLen = m_stCardInfo.EncryptFlag ? 1680 : 848;
	}
	//��ʾ
	m_myFun(FEEDBACK_ICXX, &m_stCardInfo);
}

// ���з���
int CComUnit::ProcessSendMsg()
{
	int nRes = 0;
	if (m_sendMsgList.begin() != m_sendMsgList.end())
	{
		map<DWORD, tagFrameData &>::iterator it = m_sendMsgList.begin();

		//���㷢�ʹ���
		if (m_dwSerialID == it->first)
		{
			if (m_nSendTimes++ > 3)
			{
				///ɾ��ԭ����
				m_sendMsgList.erase(it);
			}
		}
		else
		{
			m_dwSerialID = it->first;
			m_nSendTimes = 0;
		}

		nRes = m_comDev.SendData((*it).second.pFrameData, (*it).second.dwFrameDataLen);
		//���͵��豸�ɹ�
		if (nRes == TRUE)
		{
			////ɾ��ԭ����
			//m_sendMsgList.erase(it);
			return TRUE;
		}
		else
		{
			FeedbackInfo stBackInfo;
			stBackInfo.FeedResult = 9;  //�򴮿��豸����ʧ��
			m_myFun(FEEDBACK_FKXX, &stBackInfo);

			return FALSE;
		}
	}
	else
	{
		GetSendMsgReq();
	}

	return TRUE;
}


// ��ȡҪ���͵�����
int CComUnit::GetSendMsgReq()
{
	//������Ϣ
	if (m_nLocalID != 0)
	{
		m_myFun(FEEDBACK_SJREQ, &m_nLocalID);

	}

	return 0;
}


// ���÷�����Ϣ
int CComUnit::SetSendMsg(tagFrameData &Data)
{
	if (m_sendMsgList.size()> LISTMAX)
	{
		//������Ϣ
	}
	//if (m_nSendSeq >LISTMAX)
	//{
	//	m_nSendSeq = 0;
	//}
	m_sendMsgList.insert(map<DWORD, tagFrameData &>::value_type(Data.dwSerialID, Data));
	return TRUE;
}
