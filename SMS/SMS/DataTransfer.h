#pragma once
#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "DataBusiness.h"
using namespace std;
#define FALSE -1
#define TRUE 0

class CDataTransfer
{
public:
	CDataTransfer();
	~CDataTransfer();
	// ��������
	int StartZmq(int nRequest, int nRespondPort);
	// �رշ���
	int CloseZmq();

	// ��������
	int RecvData();

	// ��������
	int SendData(char *buff, unsigned long length);
protected:



	//������
	void *m_context;
	//zmqҪ��
	void *m_pRequester;
	//zmq��Ӧ
	void *m_pResponder;

	pthread_t m_ZMQRecvPt;

	CDataBusiness m_business;
};

