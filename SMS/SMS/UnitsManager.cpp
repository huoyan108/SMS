#include "UnitsManager.h"

//互斥锁
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
		case FEEDBACK_SJREQ:  //数据请求
		{
			printf("SJREQ\n");

			g_pComManager->SetDevSendMsg(*((int *)object));
			g_pComManager->ControlDevSend(*((int*)object));
						
		}
		break;
		case FEEDBACK_ICXX:	//IC信息
		{
			//DWORD LocalID;
			//char FrameNum;				//帪号
			////第0帧数据用到的变量
			//DWORD BroadcastID;			//通播ID
			//char MachineCharacter;		//用户机特征
			//unsigned short ServeFreq;			//服务频度
			//unsigned short CommLen;
			//char CommuLeave;			//通信级别
			//char EncryptFlag;			//加密标志
			//unsigned short SubordinateNum;		//下级用户总数
			////第N帧数据用到的变量
			//char CurrFrameSubNum;		//本帧下属用户数
			//DWORD lpSubUserAddrs[100];	//指向下级用户地址的指针
			CardInfoRead *pIcInfo = (CardInfoRead *)object;
			//ShowMachineMsg(m_nComID, m_stCardInfo.LocalID, m_stCardInfo.ServeFreq, m_dwCommReqLen, m_nBroadCastID);
			//修改定时发送频度
			g_pComManager->m_AlarmClock.ModifTimer(pIcInfo->cDev, pIcInfo->ServeFreq);
			printf("LocalID:%ld  ServeFreq:%d\n", pIcInfo->LocalID, pIcInfo->ServeFreq);
		}
			break;
		case FEEDBACK_ZJXX:	//自检信息
		{
			//DWORD LocalID;
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
			//		//strMsg.Format("反馈信息: %s 失败", pResult);
			//	}
			//		break;
			//	case 2:
			//	{
			//		//sprintf(strMsg.c_str, "反馈信息: %s 失败");

			//		//strMsg.Format("反馈信息:信号未锁定，未发射！");
			//	}
			//		break;
			//	case 3:
			//	{
			//		//strMsg.Format("反馈信息:电量不足，发射抑制！");
			//	}
			//		break;
			//	case 4:
			//	{
			//		//strMsg.Format("反馈信息:服务频度未到，需等待%d秒！",
			//		//pResult[0] * 1000 + pResult[1] * 100 + pResult[2] * 10 + pResult[3]);
			//	}
			//		break;
			//	case 5:
			//	{
			//		//strMsg.Format("反馈信息:加解密错误！");
			//	}
			//		break;
			//	case 6:
			//	{
			//		//strMsg.Format("反馈信息:CRC错误！%s", pResult);
			//	}
			//		break;
			//	case 7:
			//	{
			//		//strMsg.Format("反馈信息:系统抑制！");
			//	}
			//		break;
			//	case 8:
			//	{
			//		//strMsg.Format("反馈信息:无线静默！");
			//	}
			//		break;
			//	case 9:
			//	{
			//		//strMsg.Format("推送串口失败！");
			//	}
			//		break;
			//}
			g_pComManager->business.SetFeedResData(*pFeedBack);
		}

		
	}
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


	pthread_mutex_lock(&g_unitsSendData_mutex);

	m_devMap[Dev] = comDevPt;
	m_AlarmClock.AddATimer(Dev, 10);

	pthread_mutex_unlock(&g_unitsSendData_mutex);

	//自检
	SendICJC(Dev);
	SendXTZJ(Dev, 5);

	printf("Add Dev %s Success\n", Dev);
	return TRUE;
}


// 删除设备
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
	printf("modif Dev %s Success\n", Dev);

	return TRUE;
}


// 控制设备发送
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
// 向设备添加发送信息
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
	//北斗发送信息打包
	tagFrameData FrameData;
	m_parse.SendToBd_TXSQ(DataTemp, FrameData.pFrameData, FrameData.dwFrameDataLen);
	FrameData.dwSerialID = DataTemp.dwSerialID;

	if (comDevPt->SetSendMsg(FrameData) != TRUE)
	{
		return FALSE;
	}
	return TRUE;
}

//设置发送信息
int CUnitsManager::SetSendMsg(tagBdReq *pData)
{

	pthread_mutex_lock(&g_unitsSendData_mutex);

	//指定指挥机的通播
	if (pData->DestAddress == 0 && pData->SourceAddress != 0)
	{
		//指定设备加载数据
		if (SetDevSendMsg(pData->SourceAddress, pData) != TRUE)
		{
			m_dataList.push_back(*pData);
		}

	}
	//任意指挥机发送
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
// 获取设备
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


// IC卡自检
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


// 系统自检
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
