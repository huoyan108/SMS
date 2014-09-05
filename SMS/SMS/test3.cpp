//ZMQÍ¨ÐÅ
#include <unistd.h>
#include <iostream>
using namespace std;
#include "UnitsManager.h"
#define ADDDEV "AddDev"
#define DELDEV "DelDev"
#define MODIFDEV "ModifDev"
int main(int argc, char **argv)
{
	CUnitsManager manger;
	char *dev = "/dev/ttyr01";
	manger.AddComDev(dev, 115200, 8, 'N', 1);

	/*while (true)
	{
		cout << "sms>";

		char cIput[200];
		bzero(cIput, sizeof(cIput));
		cin.getline(cIput, sizeof(cIput));
		cout << cIput << endl;
	
		if (memcmp(cIput, ADDDEV, strlen(ADDDEV)) == 0)
		{
			char *dev = cIput + strlen(ADDDEV) + 1;
			//char *dev = "/dev/ttyr01";
			manger.AddComDev(dev, 115200, 8, 'N', 1);
		}
		else if (memcmp(cIput, DELDEV, strlen(DELDEV)) == 0)
		{
			char *dev = cIput + strlen(DELDEV) + 1;
			manger.DelComDev(dev);
		}
		else if (memcmp(cIput, MODIFDEV, strlen(MODIFDEV)) == 0)
		{
			char *dev = cIput + strlen(MODIFDEV) + 1;
			manger.ModifComDev(dev, 115200, 8, 'N', 1);
		}
		else
		{
			printf("what do you want to do ?\n");
		}
	}*/

	sleep(100000);

	return 1;
}
