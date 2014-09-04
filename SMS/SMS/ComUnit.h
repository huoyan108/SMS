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
	/*��ʼ����
	*@nSpeed   ����  int ����
	*@param  Dev     ����  char  �豸·��
	*@param  nBits ����  int ����λ   ȡֵ Ϊ 7 ����8
	*@param  nStop ����  int ֹͣλ   ȡֵΪ 1 ����2
	*@param  nEvent  ����  int  Ч������ ȡֵΪN,E,O,,S
	*/
	int Start(char *Dev,
		int nSpeed,
		int nBits,
		char nEvent,
		int nStop,
		FeedBack myFun);

	//�ر�
	int Stop();

	// ���з���
	int ProcessSendMsg();

	unsigned long m_nLocalID;		//ָ�ӻ�ID
	CComUnit();
	~CComUnit();


protected:
	//��������
	friend int  RecvDevData(void *object, char *DevID, char *buff, int  len);

	//���ò���ʾ�豸״̬
	void SetMachineStatus(SelfCheckInfo& stCheckInfo);
	//���ò���ʾIC״̬
	void SetIcCardMsg(CardInfoRead& stCardInfo);
	//���ò���ʾͨ�Ž��
	void SetTXRes(FeedbackInfo &stBackInfo);
	CComDev m_comDev;

	CPareData parseData;

	char m_cDev[20];

	//�����б�
	map<DWORD, tagFrameData &> m_sendMsgList;

	//�������
	DWORD m_nSendSeq;

	// ��ȡҪ���͵�����
	int GetSendMsgReq();

	//������Ϣ
	FeedBack m_myFun;

	//�Լ���Ϣ
	time_t m_tLastGetCheckMsg;
	tagSelfCheckInfo m_stCheckInfo;
	int m_nMachineStatus; //�û����Լ�״̬
	//IC��Ϣ
	CardInfoRead m_stCardInfo;
	DWORD m_dwCommReqLen; //ͨ�ų���
	int m_nBroadCastID; //ͨ����
public:

	// ���÷�����Ϣ
	int SetSendMsg(tagFrameData &Data);


};

