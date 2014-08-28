#pragma once
/*
���� �����
����	20140822
˵��	�ײ㴮��ͨ���࣬������ƴ��ڷ��ͽ��գ�ʹ��select�����¼���
*/
#include     <stdio.h>      /*��׼�����������*/
#include     <stdlib.h>     /*��׼�����ⶨ��*/
#include     <unistd.h>     /*Unix ��׼��������*/
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <fcntl.h>      /*�ļ����ƶ���*/
#include     <termios.h>    /*PPSIX �ն˿��ƶ���*/
#include     <errno.h>      /*����Ŷ���*/
#include	<pthread.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#define FALSE -1
#define TRUE 0
#define  TIMEOUT 2    //�ȴ�ʱ��2��
#define  BUFFERSIZE 512		//�����С

//�ص����������ش��豸���յ������ݡ���������ָ�룬�ն�ID���������ݣ�����
typedef int (*pf) (void *,char * ,char *,int );
class CComDev
{
public:
	CComDev();
	~CComDev();

	//���ò���	
	/*
	*@object   ����  void * ��������ָ��
	*@nSpeed   ����  int ����
	*@param  Dev     ����  char  �豸·��
	*@param  nBits ����  int ����λ   ȡֵ Ϊ 7 ����8
	*@param  nStop ����  int ֹͣλ   ȡֵΪ 1 ����2
	*@param  nEvent  ����  int  Ч������ ȡֵΪN,E,O,,S
	*/
	void SetParam(void* object,
		char *Dev,
		int nSpeed, 
		int nBits, 
		char nEvent, 
		int nStop,
		pf myFun);

	// ����ͨ�ŷ���
	int Start();

	//�رշ���
	int Stop();

	//��������
	int SendData(char *buff, int size);
	
	// �ȴ��豸����֪ͨ
	int WaitDevNotif();


protected:
	
	//���������ļ�
	int OpenDev(char *Dev);

	//���ò�������
	int set_opt(int nSpeed, int nBits, char nEvent, int nStop);

	//�ر��ļ�
	int CloseDev();

	//��ȡ����
	int RecvData();

	// �ַ�����ת��Ϊʮ��������ʾ
	int CharToHex(char *Dest, int nDestSize,
		char *Source, int nSourceLength);

	//�ص����������ش��豸���յ�������
	pf m_RecvDevDataFun;


	int m_nFd;		//�����ļ����

	pthread_t m_RecvPt;		//�����߳̾��

	char m_cDev[20];
	int m_nSpeed;
	int m_nBits;
	int m_nEvent;
	int m_nStop;

	char m_buff[BUFFERSIZE];
	long m_nBuffLength ;

	void *m_object;
};

