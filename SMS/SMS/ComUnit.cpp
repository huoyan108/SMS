#include "ComUnit.h"

int  RecvDevData(void *object,char *DevID, char *buff, int  len)
{
	CComUnit *me = (CComUnit *)object;
	if (me == NULL)
	{
		printf("CComUnit * Error\n");
		return FALSE;
	}

	//解析处理
	char pEncodeData[1000];
	bzero(pEncodeData, sizeof(pEncodeData));

	int nReportID = me->parseData.ExplainData_BD(buff, len, pEncodeData);
	switch (nReportID)
	{
		case CUNPACK_FKXX:
		{
			FeedbackInfo stBackInfo;
			stBackInfo = *(FeedbackInfo*)pEncodeData;


			//如果成功删除本地发送列表(待确定,目前确认发送就删除)
			//失败等待下次发送(待确定)
			map<DWORD, tagFrameData &>::iterator it = me->m_sendMsgList.begin();
			if (it != me->m_sendMsgList.end())
			{
				stBackInfo.dwSerialID = it->first;

				//发送时间
				time_t currtime;
				time(&currtime);
				stBackInfo.sendtime = currtime;

				//发送次数
				if (stBackInfo.dwSerialID == me->m_dwSerialID)
				{
					stBackInfo.sendtimes = me->m_nSendTimes;
				}
				

				me->m_sendMsgList.erase(it);
			}
		
			//设置反馈信息
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

	//显示
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
	//显示
	m_myFun(FEEDBACK_ICXX, &m_stCardInfo);
}

// 进行发送
int CComUnit::ProcessSendMsg()
{
	int nRes = 0;
	if (m_sendMsgList.begin() != m_sendMsgList.end())
	{
		map<DWORD, tagFrameData &>::iterator it = m_sendMsgList.begin();

		//计算发送次数
		if (m_dwSerialID == it->first)
		{
			if (m_nSendTimes++ > 3)
			{
				///删除原数据
				m_sendMsgList.erase(it);
			}
		}
		else
		{
			m_dwSerialID = it->first;
			m_nSendTimes = 0;
		}

		nRes = m_comDev.SendData((*it).second.pFrameData, (*it).second.dwFrameDataLen);
		//推送到设备成功
		if (nRes == TRUE)
		{
			////删除原数据
			//m_sendMsgList.erase(it);
			return TRUE;
		}
		else
		{
			FeedbackInfo stBackInfo;
			stBackInfo.FeedResult = 9;  //向串口设备推送失败
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


// 获取要发送的数据
int CComUnit::GetSendMsgReq()
{
	//反馈信息
	if (m_nLocalID != 0)
	{
		m_myFun(FEEDBACK_SJREQ, &m_nLocalID);

	}

	return 0;
}


// 设置发送信息
int CComUnit::SetSendMsg(tagFrameData &Data)
{
	if (m_sendMsgList.size()> LISTMAX)
	{
		//反馈信息
	}
	//if (m_nSendSeq >LISTMAX)
	//{
	//	m_nSendSeq = 0;
	//}
	m_sendMsgList.insert(map<DWORD, tagFrameData &>::value_type(Data.dwSerialID, Data));
	return TRUE;
}
