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

			unsigned long dwLocalID;
			unsigned long dwDestID;
			unsigned long dwSerialID;
			//string strMsg;

			char pSendBuffer[30];
			unsigned long dwSendDataLen;
			//������ͷ�����Ϣ
			me->SetTXRes(stBackInfo);
			me->m_myFun(FEEDBACK_FKXX, &stBackInfo);
			//����ɹ�ɾ�����ط����б�(��ȷ��,Ŀǰȷ�Ϸ��;�ɾ��)
			//ʧ�ܵȴ��´η���(��ȷ��)
			map<unsigned long, tagFrameData &>::iterator it = me->m_sendMsgList.begin();
			if (it != me->m_sendMsgList.end())
			{
				me->m_sendMsgList.erase(it);
			}
		
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

CComUnit::CComUnit() :m_nSendSeq(0),
m_myFun(0)
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
		map<unsigned long, tagFrameData &>::iterator it = m_sendMsgList.begin();
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
	m_myFun(FEEDBACK_SJREQ, m_cDev);

	return 0;
}


// ���÷�����Ϣ
int CComUnit::SetSendMsg(tagFrameData &Data)
{
	if (m_sendMsgList.size()> LISTMAX)
	{
		//������Ϣ
	}
	if (m_nSendSeq >LISTMAX)
	{
		m_nSendSeq = 0;
	}
	//m_sendMsgList[m_nSendSeq] = Data;
	//map<unsigned long, tagFrameData &>::value_type(m_nSendSeq, Data);
	m_sendMsgList.insert(map<unsigned long, tagFrameData &>::value_type(m_nSendSeq, Data));
	return TRUE;
}
