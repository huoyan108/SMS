#include "DataTransfer.h"

void *threadZMQRecv(void *arg);
CDataTransfer::CDataTransfer() :m_pResponder(NULL),
m_pRequester(NULL),
m_ZMQRecvPt(0),
m_notifFun(0),
m_nRecvLength(0)
{
	
	bzero(m_sSendSource, sizeof(m_sSendSource));
	bzero(m_nRecvBuff, sizeof(m_nRecvBuff)); 
}


CDataTransfer::~CDataTransfer()
{
}


// ��ʼ��ZMQ
int CDataTransfer::StartZmq(int nRequestPort, int nRespondPort, zmqNotif notifFun)
{
	char addr[100] = { 0 };
	bzero(addr, sizeof(addr));

	m_context = zmq_init(2);
	//m_context = zmq_ctx_new();

	//����
	m_pRequester = zmq_socket(m_context, ZMQ_PULL);
	sprintf(addr, "tcp://*:%d", nRequestPort);
	zmq_bind(m_pRequester, addr);

	//����
	m_pResponder = zmq_socket(m_context, ZMQ_PUSH);
	sprintf(addr, "tcp://*:%d", nRespondPort);
	zmq_bind(m_pResponder, addr);
	

	//���������߳�
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
#include "./proto/smsTx.pb.h"
// ��������
int CDataTransfer::RecvData()
{
	zmq_pollitem_t items[] = { { m_pRequester, 0, ZMQ_POLLIN, -1 } };
	while (TRUE)
	{
		int rt = zmq_poll(items, 1, 1);
		if (items[0].revents&ZMQ_POLLIN)
		{
			printf("zmq recv\n");

			bzero(m_sSendSource, sizeof(m_sSendSource));
			bzero(m_nRecvBuff, sizeof(m_nRecvBuff));
			m_nRecvLength = 0;

			zmq_msg_t request;
			zmq_msg_init(&request);
			zmq_msg_recv(&request, m_pRequester, 0);

			int nTotalLen = zmq_msg_size(&request);

			//��ȡ���ͷ���Դ��������ƣ�
			memcpy(m_sSendSource, zmq_msg_data(&request), sizeof(m_sSendSource));

			//��ȡ����
			char cTemp[2];
			memcpy(cTemp,
				zmq_msg_data(&request) + FSF_LEGHTH,
				sizeof(cTemp));
						
			m_nRecvLength = cTemp[0];
			m_nRecvLength *= 256;
			m_nRecvLength += cTemp[1];

			//��ȡ��������
			memcpy(m_nRecvBuff,
				zmq_msg_data(&request) + FSF_LEGHTH + SJCD_LEGHT, 
				PROCESS_FRAME_DATA_LENGTH);

			zmq_msg_close(&request);

			//protoc������������
			int nLength = strlen(m_nRecvBuff);
			if (nLength != m_nRecvLength)
			{
				continue;
			}

			BdfsMsg bdfsMsg;
			if (bdfsMsg.ParseFromString(m_nRecvBuff))
			{
				//�������ݴ����б�
				m_notifFun(m_sSendSource, (void *)&bdfsMsg);
			}
			/*CommReq *pCommReq = new CommReq();
			zmq_msg_t request;
			zmq_msg_init(&request);
			zmq_msg_recv(&request, m_pRequester, 0);

			int nTotalLen = zmq_msg_size(&request);

			//���ͷ�����
			char sSendSource[100];
			bzero(sSendSource, sizeof(sSendSource));
			memcpy(sSendSource, zmq_msg_data(&request), FSF_LEGHTH);

			memcpy(pCommReq, zmq_msg_data(&request) + FSF_LEGHTH + SJCD_LEGHT, PROCESS_FRAME_DATA_LENGTH);

			printf("ZMQReceived\n");

			//�������ݴ����б�
			m_notifFun(sSendSource, (void *)pCommReq);

			delete pCommReq;
			*/
			//zmq_msg_close(&request);
		}
	}
	return TRUE;
}


// ��������
int CDataTransfer::SendData(char *buff,DWORD length)
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
