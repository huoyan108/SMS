#include "ComDev.h"


void *threadRecv(void *arg);
CComDev::CComDev() :m_nFd(0),
m_RecvPt(0),
m_RecvDevDataFun(0),
m_nBuffLength(0)
{
	bzero(m_cDev, sizeof(m_cDev));
}


CComDev::~CComDev()
{
}

//设置参数	
void CComDev::SetParam(char *Dev, 
	int nSpeed,
	int nBits, 
	char nEvent, 
	int nStop,
	pf myFun)
{
	strcat(m_cDev, Dev);
	m_nSpeed = nSpeed;
	m_nBits = nBits;
	m_nEvent = nEvent;
	m_nStop = nStop;
	m_RecvDevDataFun = myFun;
}
// 开启通信服务
int CComDev::Start()
{
	if (OpenDev(m_cDev) != TRUE)
	{
		return FALSE;
	}
	if (set_opt(115200, 8, 'N', 1) != TRUE)
	{
		return FALSE;
	}
	//开启接收线程
	pthread_create(&m_RecvPt, NULL, threadRecv, this);

	return TRUE;
}

//关闭服务
int CComDev::Stop()
{
	//终止线程
	void * res;
	/*pthread_exit((void *)3);*/
	pthread_cancel(m_RecvPt);
	pthread_join(m_RecvPt, &res);
	printf("stop thread\n");
	//关闭串口
	CloseDev();
	return TRUE;
}
// 字符类型转换为十六进制显示
int CComDev::CharToHex(char *Dest, int nDestSize,
	char *Source, int nSourceLength)
{
	int nPos = 2;   //转换后字符位数
	for (int i = 0; i < nSourceLength; i++)
	{
		if (i >= nDestSize / nPos)
		{
			return FALSE;
		}
		sprintf(Dest + i*nPos, "%.2x", Source[i]);
	}
	Dest[nSourceLength*nPos] = '\0';
	return TRUE;
}

int CComDev::OpenDev(char *Dev)
{
	m_nFd = open(Dev, O_RDWR | O_NOCTTY | O_NDELAY);         //| O_NOCTTY | O_NDELAY
	if (-1 == m_nFd)
	{
		perror("Can't Open Serial Port");
		return FALSE;
	}
	perror("Has Open Serial Port");
	/*恢复串口为阻塞状态*/

/*	if (fcntl(m_nFd, F_SETFL, 0)<0)

		printf("fcntl failed!\n");

	else

		printf("fcntl=%d\n", fcntl(m_nFd, F_SETFL, 0));*/

	/*测试是否为终端设备*/

	if (isatty(STDIN_FILENO) == 0)
	{
		printf("standard input is not a terminal device\n");
		return FALSE;
	}
	else
	{
		printf("Is Terminal:YES\n");
		return TRUE;
	}
	return TRUE;

}
int CComDev::set_opt(int nSpeed, int nBits, char nEvent, int nStop)
{

	struct termios newtio, oldtio;

	/*保存测试现有串口参数设置，在这里如果串口号等出错，会有相关的出错信息*/
	if (tcgetattr(m_nFd, &oldtio) != 0) {
		perror("SetupSerial 1");
		return FALSE;
	}

	bzero(&newtio, sizeof(newtio));
	/*步骤一，设置字符大小*/

	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;

	/*设置停止位*/
	switch (nBits)
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}

	/*设置奇偶校验位*/
	switch (nEvent)
	{
	case 'O': //奇数
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E': //偶数
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':  //无奇偶校验位
		newtio.c_cflag &= ~PARENB;
		break;
	}

	/*设置波特率*/
	switch (nSpeed)
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	case 460800:
		cfsetispeed(&newtio, B460800);
		cfsetospeed(&newtio, B460800);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}

	/*设置停止位*/
	if (nStop == 1)
		newtio.c_cflag &= ~CSTOPB;
	else if (nStop == 2)
		newtio.c_cflag |= CSTOPB;

	/*设置等待时间和最小接收字符*/
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;

	/*处理未接收字符*/
	tcflush(m_nFd, TCIFLUSH);

	/*激活新配置*/
	if ((tcsetattr(m_nFd, TCSANOW, &newtio)) != 0)
	{
		perror("Com set error");
		return FALSE;
	}

	printf("Com set done!\n");
	return TRUE;
}
void *threadRecv(void *arg)
{
	pthread_detach(pthread_self());

	int state, oldstate;

	state = PTHREAD_CANCEL_DEFERRED;
	pthread_setcancelstate(state, &oldstate);

	CComDev *me = (CComDev *)arg;
	while (true)
	{
 		if (me->WaitDevNotif() != TRUE)
		{
			break;
		}
	pthread_testcancel();

	}
	return NULL;
}

int CComDev::RecvData()
{
	unsigned long nRead = 0;

	char buff[BUFFERSIZE];
	bzero(buff, sizeof(buff));

	char cShow[BUFFERSIZE];
	bzero(cShow, sizeof(buff));
	
	

	while((nRead = read(m_nFd, buff, sizeof(buff))) > 0)
	{
		memcpy(m_buff + m_nBuffLength, buff, nRead);
		m_nBuffLength += nRead;
	}
	m_buff[m_nBuffLength] = '\0';

	printf("RECV CHAR:%s\n", m_buff);

	if (CharToHex(cShow, BUFFERSIZE, m_buff, m_nBuffLength) == TRUE)
	{
		printf("RECV HEX:%s\n", cShow);
	}
	else
	{
		printf("\nCharToHex:Error!");
	}
	//存入缓存 或通知 待加
	m_RecvDevDataFun(m_cDev,m_buff, m_nBuffLength);

	m_nBuffLength = 0;

	return TRUE;
}
// 等待设备通知
int CComDev::WaitDevNotif()
{
	int fd; //文件描述符
	fd = m_nFd;

	int rtv; //存放返回值
	fd_set fs_read; //读状态数据集
	struct timeval  timeout;//超时设置参数结构体
	timeout.tv_usec = TIMEOUT;  //毫秒
	FD_ZERO(&fs_read); //数据集清零
	FD_SET(fd, &fs_read);//绑定描述符与数据集
	rtv = select(fd + 1, &fs_read, NULL, NULL, &timeout);//查询改描述符关联设备是否存在数据可读
	if (rtv < 0)//select函数调用失败
	{
		perror("select() Error!");
	}
	else if (rtv) //存在可读数据，准备读取
	{
		RecvData();
	}
	else  //超时，其他意外
	{
	}
	
	return TRUE;
}


int CComDev::CloseDev()
{
	
	close(m_nFd);
	printf("close serial port");
	return TRUE;
}

int CComDev::SendData(char *buff, int size)
{
	printf("\nSEND CHAR:%s", buff);

	if (write(m_nFd, buff, size) != TRUE)
		return FALSE;
	else
		return TRUE;
}
