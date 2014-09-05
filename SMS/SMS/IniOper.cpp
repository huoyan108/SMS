#include "IniOper.h"


CIniOper::CIniOper()
{
}


CIniOper::~CIniOper()
{
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
