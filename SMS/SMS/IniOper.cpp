#include "IniOper.h"


CIniOper::CIniOper()
{
}


CIniOper::~CIniOper()
{
}
int  CIniOper::loadComDev(char *filename, vector<COM_DEV_TAG> &devArr)
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
	{
		return -1;
	}
	
	//���ڲ�������
	lua_getglobal(L, "count");
	if (!lua_isnumber(L, -1))
	{
		return -1;
	}
	int nCount = 0;
	nCount = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);


	lua_getglobal(L, "devs");
	if (!lua_istable(L, -1))
	{
		return -1;
	}

	//lua_pushnumber(L,1);
	//lua_gettable(L, -2);

	//if (!lua_isstring(L, -1))
	//{
	//	return -1;
	//}
	//COM_DEV_TAG devTag;

	//sprintf(devTag.Dev, "%s", (char *)lua_tostring(L, -1));
	//��ȡ��table
	for (int i = 0; i < nCount; i++)
	{
		lua_pushnumber(L, i + 1); //����

		lua_gettable(L, -2);  //ȥtable����

		if (!lua_istable(L, -1))	//����ǲ���һ����table
		{
			return -1;
		}
		//��ȡ��table����
		COM_DEV_TAG devTag;
		if (GetDevTableValue(L, devTag, 1) != 0)
		{
			return -1;
		}
		devArr.insert(devArr.begin(), devTag);
		lua_pop(L, 1);	//�ɵ���table
	}
	lua_pop(L, 1);	//�ɵ�table

	lua_close(L);
	return 0;
}

int  CIniOper::load(char *filename, int *nRequestPort, int *nRespondPort, char *pSoftName)
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
	{
		//error(L, "cannot run configuration file: %s",
		//lua_tostring(L, -1));
		return -1;
	}


	lua_getglobal(L, "RequestPort");
	lua_getglobal(L, "RespondPort");
	lua_getglobal(L, "softname");

	if (!lua_isnumber(L, -3))
	{
		//error(L, "`RequestPort' should be a number\n");
		return -1;

	}
	if (!lua_isnumber(L, -2))
	{
		//error(L, "`RequestPort' should be a number\n");
		return -1;

	}
	if (!lua_isstring(L, -1))
	{
		//error(L, "`RespondPort' should be a number\n");
		return -1;
	}

	*nRequestPort = (int)lua_tonumber(L, -3);
	*nRespondPort = (int)lua_tonumber(L, -2);
	sprintf(pSoftName, "%s", (char *)lua_tostring(L, -1));
	lua_pop(L, 3);

	lua_close(L);
	return 0;
}

int  CIniOper::load(char *filename, char *dbhost, char * dbport, char * dbname, char * dbuser, char * dbpwd)
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
	{
		//error(L, "cannot run configuration file: %s",
		//lua_tostring(L, -1));
		return -1;
	}


	lua_getglobal(L, "pghost");
	lua_getglobal(L, "pgport");
	lua_getglobal(L, "dbName");
	lua_getglobal(L, "login");
	lua_getglobal(L, "passwd");

	if (!lua_isstring(L, -5))
	{
		//error(L, "`RequestPort' should be a number\n");
		return -1;

	}
	if (!lua_isstring(L, -4))
	{
		//error(L, "`RequestPort' should be a number\n");
		return -1;

	}
	if (!lua_isstring(L, -3))
	{
		//error(L, "`RequestPort' should be a number\n");
		return -1;

	}
	if (!lua_isstring(L, -2))
	{
		//error(L, "`RequestPort' should be a number\n");
		return -1;

	}
	if (!lua_isstring(L, -1))
	{
		//error(L, "`RespondPort' should be a number\n");
		return -1;
	}
	sprintf(dbpwd, "%s", (char *)lua_tostring(L, -1));
	sprintf(dbuser, "%s", (char *)lua_tostring(L, -2));
	sprintf(dbname, "%s", (char *)lua_tostring(L, -3));
	sprintf(dbport, "%s", (char *)lua_tostring(L, -4));
	sprintf(dbhost, "%s", (char *)lua_tostring(L, -5));

	lua_pop(L, 5);

	lua_close(L);
	return 0;
}

int CIniOper::GetSoftPath(char *cPath,int nSize)
{
	int n;

	n = readlink("/proc/self/exe", cPath, nSize);
	if (n < 0 && n > nSize)
	{
		printf("get software path error ,no file or pathlength is too long");
		return -1;
	}

	int i;
	for (i = n; i >= 0; --i)
	{
		if (cPath[i] == '/')
		{
			cPath[i + 1] = '\0';
			printf("software path:%s\n", cPath);
			return 0;
		}
	}
	return -1;
}


// ��ȡ�豸����table������
int CIniOper::GetDevTableValue(lua_State *L, COM_DEV_TAG &devTag,int tablePos)
{
	//dev
	lua_pushstring(L, "dev"); //����
	lua_gettable(L, -2);  //ȥtable����
	if (!lua_isstring(L, -1))	
	{
		return -1;
	}
	sprintf(devTag.Dev, "%s", (char *)lua_tostring(L, -1));
	lua_pop(L, 1);

	lua_pushstring(L, "speed"); //����
	lua_gettable(L, -2);  //ȥtable����
	if (!lua_isnumber(L, -1))
	{
		return -1;
	}
	devTag.nSpeed = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "bits"); //����
	lua_gettable(L, -2);  //ȥtable����
	if (!lua_isnumber(L, -1))
	{
		return -1;
	}
	devTag.nBits = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushstring(L, "event"); //����
	lua_gettable(L, -2);  //ȥtable����
	if (!lua_isstring(L, -1))
	{
		return -1;
	}
	sprintf(&devTag.nEvent, "%s", (char *)lua_tostring(L, -1));
	//devTag.nEvent = (char)lua_tostring(L, -1));
	lua_pop(L, 1);

	lua_pushstring(L, "stop"); //����
	lua_gettable(L, -2);  //ȥtable����
	if (!lua_isnumber(L, -1))
	{
		return -1;
	}
	devTag.nStop = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);
	return 0;
}
