#pragma once
#ifdef __cplusplus             //���߱��������ⲿ�ִ��밴C���Եĸ�ʽ���б��룬������C++��

extern "C"{

#endif

	/*��*/
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#ifdef __cplusplus

}

#endif

#include <stdio.h>
#include <unistd.h>

class CIniOper
{
public:
	CIniOper();
	~CIniOper();
	int GetSoftPath(char *cPath, int nSize);
	int load(char *filename, int *nRequestPort, int *nRespondPort);
};

