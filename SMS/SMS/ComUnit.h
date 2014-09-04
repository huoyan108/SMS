#pragma once
#include <map>
#include "ComDev.h"
#include "PareData.h"
#include "MsgStructInfo.h"
#include <time.h>
using namespace std;
#define FALSE -1
#define TRUE 0
#define LISTMAX 1000
typedef int (* FeedBack)(int, void *);
class CComUnit
{
public:
	/*开始运行
	*@nSpeed   类型  int 速率
	*@param  Dev     类型  char  设备路径
	*@param  nBits 类型  int 数据位   取值 为 7 或者8
	*@param  nStop 类型  int 停止位   取值为 1 或者2
	*@param  nEvent  类型  int  效验类型 取值为N,E,O,,S
	*/
	int Start(char *Dev,
		int nSpeed,
		int nBits,
		char nEvent,
		int nStop,
		FeedBack myFun);

	//关闭
	int Stop();

	// 进行发送
	int ProcessSendMsg();

	unsigned long m_nLocalID;		//指挥机ID
	CComUnit();
	~CComUnit();


protected:
	//接收数据
	friend int  RecvDevData(void *object, char *DevID, char *buff, int  len);

	//设置并显示设备状态
	void SetMachineStatus(SelfCheckInfo& stCheckInfo);
	//设置并显示IC状态
	void SetIcCardMsg(CardInfoRead& stCardInfo);
	//设置并显示通信结果
	void SetTXRes(FeedbackInfo &stBackInfo);
	CComDev m_comDev;

	CPareData parseData;

	char m_cDev[20];

	//发送列表
	map<DWORD, tagFrameData &> m_sendMsgList;

	//发送序号
	DWORD m_nSendSeq;

	// 获取要发送的数据
	int GetSendMsgReq();

	//返回信息
	FeedBack m_myFun;

	//自检信息
	time_t m_tLastGetCheckMsg;
	tagSelfCheckInfo m_stCheckInfo;
	int m_nMachineStatus; //用户机自检状态
	//IC信息
	CardInfoRead m_stCardInfo;
	DWORD m_dwCommReqLen; //通信长度
	int m_nBroadCastID; //通播号
public:

	// 设置发送信息
	int SetSendMsg(tagFrameData &Data);


};

