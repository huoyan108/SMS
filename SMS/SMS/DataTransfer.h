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
	// 开启服务
	int StartZmq(int nRequest, int nRespondPort);
	// 关闭服务
	int CloseZmq();

	// 接收数据
	int RecvData();

	// 发送数据
	int SendData(char *buff, unsigned long length);
protected:



	//上下文
	void *m_context;
	//zmq要求
	void *m_pRequester;
	//zmq回应
	void *m_pResponder;

	pthread_t m_ZMQRecvPt;

	CDataBusiness m_business;
};

