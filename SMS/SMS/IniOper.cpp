#include "IniOper.h"


CIniOper::CIniOper()
{
}


CIniOper::~CIniOper()
{
}

int  CIniOper::load(char *filename, int *nRequestPort, int *nRespondPort)
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
	if (!lua_isnumber(L, -2))
	{
		//error(L, "`RequestPort' should be a number\n");
		return -1;

	}
	if (!lua_isnumber(L, -1))
	{
		//error(L, "`RespondPort' should be a number\n");
		return -1;
	}
	*nRequestPort = (int)lua_tonumber(L, -2);
	*nRespondPort = (int)lua_tonumber(L, -1);

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
