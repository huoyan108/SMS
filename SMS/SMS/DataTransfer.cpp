#include "DataTransfer.h"

void *threadZMQRecv(void *arg);
//CDataTransfer *g_pDatatransfer = NULL;
CDataTransfer::CDataTransfer() :m_pResponder(NULL),
m_pRequester(NULL),
m_ZMQRecvPt(0),
m_notifFun(0)
{
	//g_pDatatransfer = this;
}


CDataTransfer::~CDataTransfer()
{
}


// 初始化ZMQ
int CDataTransfer::StartZmq(int nRequestPort, int nRespondPort, zmqNotif notifFun)
{
	char addr[100] = { 0 };
	bzero(addr, sizeof(addr));

	m_context = zmq_init(2);
	//m_context = zmq_ctx_new();

	//接收
	m_pRequester = zmq_socket(m_context, ZMQ_PULL);
	sprintf(addr, "tcp://*:%d", nRequestPort);
	zmq_bind(m_pRequester, addr);

	//发送
	m_pResponder = zmq_socket(m_context, ZMQ_PUSH);
	sprintf(addr, "tcp://*:%d", nRespondPort);
	zmq_bind(m_pResponder, addr);
	

	//创建接收线程
	pthread_create(&m_ZMQRecvPt, NULL, threadZMQRecv, this);

	m_notifFun = notifFun;
	return 0;
}

void *threadZMQRecv(void *arg)
{
	pthread_detach(pthread_self());

	int state, oldstate;

	state = PTHREAD_CANCEL_DEFERRED;
	pthread_setcancelstate(state, &oldstate);

	CDataTransfer *me = (CDataTransfer *)arg;
	while (true)
	{
		if (me->RecvData() != TRUE)
		{
			break;
		}
		pthread_testcancel();

	}
	return NULL;
}

// 接收数据
int CDataTransfer::RecvData()
{
	zmq_pollitem_t items[] = { { m_pRequester, 0, ZMQ_POLLIN, -1 } };
	while (TRUE)
	{
		int rt = zmq_poll(items, 1, 1);
		if (items[0].revents&ZMQ_POLLIN)
		{
			printf("zmq recv\n");

			CommReq *pCommReq = new CommReq();
			zmq_msg_t request;
			zmq_msg_init(&request);
			zmq_msg_recv(&request, m_pRequester, 0);

			int nTotalLen = zmq_msg_size(&request);

			//发送方名称
			char sSendSource[100];
			bzero(sSendSource, sizeof(sSendSource));
			memcpy(sSendSource, zmq_msg_data(&request), FSF_LEGHTH);

			//pCommReq->dwFrameDataLen = nTotalLen - FSF_LEGHTH - SJCD_LEGHT;
			memcpy(pCommReq, zmq_msg_data(&request) + FSF_LEGHTH + SJCD_LEGHT, PROCESS_FRAME_DATA_LENGTH);
			printf("ZMQReceived\n");

			//存入数据处理列表
			//m_business.SetBusiessData(pCommReq);
			m_notifFun(sSendSource, (void *)pCommReq);
			zmq_msg_close(&request);
		}
	}
	return TRUE;
}


// 发送数据
int CDataTransfer::SendData(char *buff,unsigned long length)
{
	zmq_msg_t reply;
	zmq_msg_init_size(&reply, length);
	memcpy(zmq_msg_data(&reply), buff, length);
	zmq_msg_send(&reply, m_pResponder, 0);
	zmq_msg_close(&reply);
	return TRUE;
}


// 关闭ZMQ
int CDataTransfer::CloseZmq()
{
	//终止线程
	void * res;
	pthread_cancel(m_ZMQRecvPt);
	pthread_join(m_ZMQRecvPt, &res);
	printf("stop m_ZMQRecvPt thread\n");

	zmq_close(m_pRequester);
	zmq_close(m_pResponder);
	zmq_ctx_destroy(m_context);
	return 0;
}
