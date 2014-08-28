#ifndef _MSGSTRUCTINFO_H_
#define _MSGSTRUCTINFO_H_

struct _SEND_DATA_TAG
{
	unsigned int nSeq;

};

#define TIME_CHECK_SELF 5 //�Լ�Ƶ��Ϊ5��

#define PARAME_LENGTH 50 //�ַ��������ĳ���

#define LIST_COUNT_WAIT_FOR_SEND  20  //�����д�ŵ�����

#define PROCESS_FRAME_DATA_LENGTH  1024*7 //���ݴ���֡����

enum DATA_FROM_TYPE
{
	DATA_FROM_TCP = 1, //TCP���ӷ��͵�����
	DATA_FROM_UDP,
	DATA_FROM_BD
};
enum {
	FEEDBACK_SJREQ,    //��������
	FEEDBACK_ICXX,		//IC��Ϣ   
	FEEDBACK_ZJXX,		//�Լ���Ϣ
	FEEDBACK_FKXX		//������Ϣ
};
enum{
	CUNPACK_NULL = 0,
	CUNPACK_DWXX,		//��λ��Ϣ 01
	CUNPACK_TXXX,		//ͨ����Ϣ 02
	CUNPACK_TXHZ,		//ͨ�Ż�ִ 03
	CUNPACK_ICXX,		//IC��Ϣ   04
	CUNPACK_ZJXX,		//�Լ���Ϣ 05
	CUNPACK_FKXX		//������Ϣ 06
};

enum
{
	CUNPACK_TXFS_DS = 1, //ͨ�ŷ���
	CUNPACK_XTZJ_DS, //ϵͳ�Լ�
	CUNPACK_XTYD_DS  //����Ӧ��
};

//������Ϣ
enum
{
	OPERATE_PUSH_SUCCESS = 0, //�ύ�ɹ�
	OPERATE_SEND_SUCCESS = 3, //���ͳɹ�
	OPERATE_PUSH_FAIL = 2  //�ύʧ��
};

//�û���״̬
enum
{
	MACHINE_STATUS_COM_UNOPEN = 1, //����δ��
	MACHINE_STATUS_WORK, //��������
	MACHINE_STATUS_MASK //����״̬
};

/************************************************************************/
//ָ�ӻ�������                                                     
/************************************************************************/
typedef struct tagCardInfoRead
{
	unsigned long LocalID;
	char FrameNum;				//����
	//��0֡�����õ��ı���
	unsigned long BroadcastID;			//ͨ��ID
	char MachineCharacter;		//�û�������
	unsigned short ServeFreq;			//����Ƶ��
	unsigned short CommLen;
	char CommuLeave;			//ͨ�ż���
	char EncryptFlag;			//���ܱ�־
	unsigned short SubordinateNum;		//�¼��û�����
	//��N֡�����õ��ı���
	char CurrFrameSubNum;		//��֡�����û���
	unsigned long lpSubUserAddrs[100];	//ָ���¼��û���ַ��ָ��
} CardInfoRead, *lpCardInfoRead;

//�Լ���Ϣ
typedef struct tagSelfCheckInfo
{
	unsigned long LocalID;
	char ICCardState;		// IC��״̬
	char HardState;		// Ӳ��״̬
	char Batterystate;		// ��ص���
	char InState;			// ��վ״̬
	char PowerState[6];	// ����״��
}SelfCheckInfo, *lpSelfCheckInfo;

//������Ϣ
typedef struct tagFeedbackInfo
{
	unsigned long LocalID;
	char FeedResult; //�������
	char Reserve[10];   // ����
}FeedbackInfo, *lpFeedbackInfo;

/************************************************************************/
///�û���������                                                     
/************************************************************************/
//ͨ�Ż�ִ
typedef struct tagSendBackInfo
{
	unsigned long dwSendID;
	unsigned long dwRecvID;
	char nHour;
	char nMinute;
	char nSecond;
}SendBackInfo, *lpSendBackInfo;

//������λ��Ϣ
typedef struct  tagPosInfo
{
	char  ifSecret;	//0:����Կ  1:����Կ
	char  Precision;	//0:һ��(20m)  1:����(100m)
	char  ifUrgent;		//0:��  1:����
	char  ifMultiValue;	//0:��ֵ��  1:��
	unsigned long LocalID;	//ָ�����û�����ѯ�û����û���ַ

	char pSecrets[6]; //��Կ

	char  Hour;		// ��λʱ��--ʱ
	char  Minute;	// ��λʱ��--��
	char  Second;	// ��λʱ��--��
	char  CentiSecond;	// ��λʱ��--0.01��
	char  LongDeg;	//���ȣ���
	char  LongMin;	//��
	char  LongSec;	//��
	char  LongDecsec;	//0.1��
	char  LatDeg;	//γ�ȣ���
	char  LatMin;	//��
	char  LatSec;	//��
	char  LatDecsec;	//0.1��

	char nAltSign; //�̷߳���,0����1,��
	unsigned long Altitude;	//�߳�
	char nEfSign; //�߳��쳣����
	unsigned long Ef;		//�߳��쳣
}PosInfo, *lpPosInfo;

//ͨ����Ϣ
typedef struct tagCommInfo
{
	unsigned long LocalID;	//����ID(���շ�ID�������û�)
	char  ifBCD;	//0:����  1:���� 
	char  ifUrgent; //0:�ؿ�  1����ͨ
	int  ifSecret;	//0:����Կ 1:����Կ
	unsigned long SrcAddress;	//���ŷ���ַ
	char  CommHour;	//
	char  CommMin;	//
	char CommSecond; //
	unsigned short CommLenByte;
	unsigned long CommLenBit;	//���ĳ���(��λ��bit)  ���ܴ��ֽ�������ΪBCD���ܲ������ֽ�
	char  CommBuff[211];
	int  CRCFlag;	//00:CRCУ����ȷ 01:CRCУ�����
}CommInfo, *lpCommInfo;

/************************************************************************/
///���赽�û���                                                       
/************************************************************************/
//ͨ������
typedef struct tagCommReq
{
	unsigned long LocalID;
	unsigned long DestAddress;		//���շ���ַ
	unsigned long InfoLen;			//���ݳ���(��λ��Bit)  ���ܴ��ֽ�������ΪBCD���ܲ������ֽ�
	char  InfoBuff[211];	//����:Max 120��  BCD:Max 420
	//����ͨ��
	int  ifSecret;			//�̶�����
	int  ifQuick;			//00:��ͨ   01:�ؿ�[ע����Э��涨�෴]
	int  ifBCD;			//0:����    1:����
	int  ifCmd;			//[��׼Э��2.1���޴���] ����ʶ��[0:ͨ��  1:����ʶ��]
	int  ifAnswer;			//[��׼Э��2.1���޴���] �Ƿ�Ӧ��

	//
	unsigned long dwSerialID; //ͨ����ˮ��
	unsigned long dwTerminalID; //�ն˺�
	//
	char nSended; //�Ƿ���
}CommReq, *lpCommReq;

typedef struct tagClientCheck
{
	unsigned long dwAnswerIndex;
}ClientCheck,*lpClientCheck;

/************************************************************************/
///ϵͳ�ṹ��                                                       
/************************************************************************/
#include <time.h>
typedef struct tagSendDataInfo
{

	tagCommReq stCommReq; //ͨ������ṹ��

	time_t tRecvTime;
	time_t tSendTime;

	char nSended;
	char nRecved;

	unsigned long dwIndexInTable; //���е�����
}SendDataInfo, *lpSendDataInfo;

//ϵͳ��ʼ���ṹ��
typedef struct tagSysInial
{
	unsigned long dwListenPort_TCP;
	unsigned long dwListenPort_UDP;
	char pServerName[50];
	char pDbName[50];
	char pUserName[50];

	unsigned long dwListCount; //�û�������������
}SysInial;

//���ݴ���ṹ��
struct tagFrameData
{
	char pFrameData[PROCESS_FRAME_DATA_LENGTH];
	unsigned long dwFrameDataLen; //���ݳ���
	DATA_FROM_TYPE eFromType; //������Դ
	char nComID; //���ں�
	char cDev[20];
};
#endif
