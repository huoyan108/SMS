//´®¿Ú²âÊÔ
#include "ComDev.h"
int  RecvData(void *object,char *DevID,char *buff, int  len)
{
	printf("\nRECV Notif\n");

	return true;
}
int main(int argc, char **argv)
{
	CComDev m_comDev;

	char *dev = "/dev/ttyr01";

	m_comDev.SetParam(NULL,dev, 115200, 8, 'N', 1, RecvData);
	
	m_comDev.Start();
 	char cmsg[] = { 0x24,0x49,0x43,0x4A,0x43,0x00,0x0C,0x00,0x00,0x00,0x00,0x2B};
	//m_comDev.SendData(cmsg, 12);
	for (int i = 0; i < 10; i++)
	{
		m_comDev.SendData(cmsg, 12);
		sleep(1);
	}

	sleep(10000000);
	m_comDev.Stop();

	return 1;
}
