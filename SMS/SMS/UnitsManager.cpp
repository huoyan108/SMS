#include "UnitsManager.h"

//互斥锁
pthread_mutex_t g_unitsSendData_mutex = PTHREAD_MUTEX_INITIALIZER;

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
		case FEEDBACK_SJREQ:  //数据请求
		{
			printf("SJREQ\n");

			g_pComManager->SetDevSendMsg((char *)object);
			g_pComManager->ControlDevSend((char *)object);
						
		}
		break;
		case FEEDBACK_ICXX:	//IC信息
		{
			//unsigned long LocalID;
			//char FrameNum;				//帪号
			////第0帧数据用到的变量
			//unsigned long BroadcastID;			//通播ID
			//char MachineCharacter;		//用户机特征
			//unsigned short ServeFreq;			//服务频度
			//unsigned short CommLen;
			//char CommuLeave;			//通信级别
			//char EncryptFlag;			//加密标志
			//unsigned short SubordinateNum;		//下级用户总数
			////第N帧数据用到的变量
			//char CurrFrameSubNum;		//本帧下属用户数
			//unsigned long lpSubUserAddrs[100];	//指向下级用户地址的指针
			CardInfoRead *pIcInfo = (CardInfoRead *)object;
			//ShowMachineMsg(m_nComID, m_stCardInfo.LocalID, m_stCardInfo.ServeFreq, m_dwCommReqLen, m_nBroadCastID);
			printf("LocalID:%ld  ServeFreq:%ld\n", pIcInfo->LocalID, pIcInfo->ServeFreq);
		}
			break;
		case FEEDBACK_ZJXX:	//自检信息
		{
			//unsigned long LocalID;
			//char ICCardState;		// IC卡状态
			//char HardState;		// 硬件状态
			//char Batterystate;		// 电池电量
			//char InState;			// 入站状态
			//char PowerState[6];	// 功率状况
			SelfCheckInfo *pCheck = (SelfCheckInfo *)object;
			printf("ICCardState:%d  HardState:%d  Batterystate:%d InState:%d  PowerState:%s\n",
				pCheck->ICCardState, pCheck->HardState, pCheck->Batterystate, pCheck->InState, pCheck->PowerState);
		}
			break;
		case FEEDBACK_FKXX:	//反馈信息
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
						sprintf(cMsg, "TX SUCCESS\n");
						return TRUE;
					}
				}
					break;
				case 1:
				{
					//strMsg.Format("反馈信息: %s 失败", pResult);
				}
					break;
				case 2:
				{
					//sprintf(strMsg.c_str, "反馈信息: %s 失败");

					//strMsg.Format("反馈信息:信号未锁定，未发射！");
				}
					break;
				case 3:
				{
					//strMsg.Format("反馈信息:电量不足，发射抑制！");
				}
					break;
				case 4:
				{
					//strMsg.Format("反馈信息:服务频度未到，需等待%d秒！",
					//pResult[0] * 1000 + pResult[1] * 100 + pResult[2] * 10 + pResult[3]);
				}
					break;
				case 5:
				{
					//strMsg.Format("反馈信息:加解密错误！");
				}
					break;
				case 6:
				{
					//strMsg.Format("反馈信息:CRC错误！%s", pResult);
				}
					break;
				case 7:
				{
					//strMsg.Format("反馈信息:系统抑制！");
				}
					break;
				case 8:
				{
					//strMsg.Format("反馈信息:无线静默！");
				}
					break;
			}
		}

		
	}
	//g_pComManager->m_devMap["ad"];
	return TRUE;
}
// 添加设备
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
	m_AlarmClock.AddATimer(Dev, 10);
	return TRUE;
}


// 删除设备
int CUnitsManager::DelComDev(char *Dev)
{
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
	return TRUE;
}


// 修改设备
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


// 控制设备发送
int CUnitsManager::ControlDevSend(char *Dev/*, char *buff, int size*/)
{
	map<string, CComUnit*>::iterator it = m_devMap.find(Dev);
	if (it != m_devMap.end())
	{
		CComUnit *comDevPt = it->second;
	
		//CommInfo Data;
		//Data.dwFrameDataLen = 12;
		//Data.pFrameData = { 0x24, 0x49, 0x43, 0x4A, 0x43, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x2B };
		//comDevPt->SetSendMsg(Data);
		comDevPt->ProcessSendMsg();
	}
	return 0;
}


// 向设备添加发送信息
int CUnitsManager::SetDevSendMsg(char *DevID, CommInfo *pData)
{
	CComUnit *comDevPt = NULL;
	if (g_pComManager->GetComDev(&comDevPt, DevID) != TRUE)
	{
		return FALSE;
	}

	if (pData != NULL)
	{
		if (comDevPt->SetSendMsg(*pData) != TRUE)
		{
			return FALSE;
		}
		return TRUE;
	}
	pthread_mutex_lock(&g_unitsSendData_mutex);

	CommInfo Data = m_dataList.front();
	m_dataList.pop_front();
	pthread_mutex_unlock(&g_unitsSendData_mutex);

	if (comDevPt->SetSendMsg(Data) != TRUE)
	{
		return FALSE;
	}
	return TRUE;
}

//设置发送信息
int CUnitsManager::SetSendMsg(CommInfo &Data)
{
	pthread_mutex_lock(&g_unitsSendData_mutex);

	//制定的直接放进设备,否则先存在列表
	if (Data.cDev != NULL)
	{
		if (SetDevSendMsg(Data.cDev, &Data) != TRUE)
		{
			m_dataList.push_back(Data);
		}

	}
	else
	{
		m_dataList.push_back(Data);
	}

	pthread_mutex_unlock(&g_unitsSendData_mutex);

	return TRUE;
}
// 获取设备
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
