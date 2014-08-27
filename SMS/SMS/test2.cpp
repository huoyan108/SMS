//¿ØÖÆµ¥Ôª²âÊÔ
#include "UnitsManager.h"

int main(int argc, char **argv)
{
	CUnitsManager manger;
	char *dev = "/dev/ttyr01";
	manger.AddComDev(dev, 115200, 8, 'N', 1);
	manger.ControlDevSend(dev);

	sleep(10000000);
	

	return 1;
}
