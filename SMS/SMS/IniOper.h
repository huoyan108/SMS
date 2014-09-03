#pragma once
#ifdef __cplusplus             //告诉编译器，这部分代码按C语言的格式进行编译，而不是C++的

extern "C"{

#endif

	/*…*/
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

