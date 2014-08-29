#include "DataTransfer.h"

void *threadZMQRecv(void *arg);

CDataTransfer::CDataTransfer() :m_pResponder(NULL),
m_pRequester(NULL),
m_ZMQRecvPt(0)
{
}


CDataTransfer::~CDataTransfer()
{
}


// ��ʼ��ZMQ
int CDataTransfer::StartZmq(int nRequest, int nRespondPort)
{
	char addr[16] = { 0 };
	bzero(addr, sizeof(addr));

	m_context = zmq_init(2);

	//����
	m_pRequester = zmq_socket(m_context, ZMQ_PULL);
	sprintf(addr, "tcp://*:%s", nRequest);
	zmq_bind(m_pRequester, addr);

	//����
	m_pResponder = zmq_socket(m_context, ZMQ_PUSH);
	sprintf(addr, "tcp://*:%s", nRespondPort);
	zmq_bind(m_pResponder, addr);
	

	//���������߳�
	pthread_create(&m_ZMQRecvPt, NULL, threadZMQRecv, this);
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

// ��������
int CDataTransfer::RecvData()
{
	zmq_pollitem_t items[] = { { m_pRequester, 0, ZMQ_POLLIN, -1 } };
	while (TRUE)
	{
		int rt = zmq_poll(items, 1, -1);
		if (items[0].revents&ZMQ_POLLIN)
		{
			tagFrameData *pTagFrameData = new tagFrameData();
			zmq_msg_t request;
			zmq_msg_init(&request);
			zmq_msg_recv(&request, m_pRequester, 0);

			int nTotalLen = zmq_msg_size(&request);

			//���ͷ�����
			char sSendSource[100];
			bzero(sSendSource, sizeof(sSendSource));
			memcpy(sSendSource, zmq_msg_data(&request), FSF_LEGHTH);

			pTagFrameData->dwFrameDataLen = nTotalLen - FSF_LEGHTH - SJCD_LEGHT;
			memcpy(pTagFrameData->pFrameData, zmq_msg_data(&request) + FSF_LEGHTH + SJCD_LEGHT, PROCESS_FRAME_DATA_LENGTH);
			printf("ZMQReceived\n");

			//�������ݴ����б�
			m_business.SetBusiessData(pTagFrameData);
			zmq_msg_close(&request);
		}
	}
	return TRUE;
}


// ��������
int CDataTransfer::SendData(char *buff,unsigned long length)
{
	zmq_msg_t reply;
	zmq_msg_init_size(&reply, length);
	memcpy(zmq_msg_data(&reply), buff, length);
	zmq_msg_send(&reply, m_pResponder, 0);
	zmq_msg_close(&reply);
	return TRUE;
}


// �ر�ZMQ
int CDataTransfer::CloseZmq()
{
	//��ֹ�߳�
	void * res;
	pthread_cancel(m_ZMQRecvPt);
	pthread_join(m_ZMQRecvPt, &res);
	printf("stop m_ZMQRecvPt thread\n");

	zmq_close(m_pRequester);
	zmq_close(m_pResponder);
	zmq_ctx_destroy(m_context);
	return 0;
}