//控制单元测试
#include "UnitsManager.h"

int main(int argc, char **argv)
{
	CUnitsManager manger;
	char *dev = "/dev/ttyr01";
	manger.AddComDev(dev, 115200, 8, 'N', 1);

	//测试用
	CommInfo Data;
	Data.dwFrameDataLen = 12;
	Data.pFrameData = { 0x24, 0x49, 0x43, 0x4A, 0x43, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x2B };

	manger.SetSendMsg(Data);
	//manger.ControlDevSend(dev);
	//for (int i = 0; i < 10; i++)
	{
		//manger.ControlDevSend(dev);
		sleep(1);
	}
	sleep(10000000);
	

	return 1;
}
