#pragma once
#include <vector>
#include "MsgStructInfo.h"

using namespace std;
class CMyVector :
	public vector<SENDDATA_TAG>
{
public:
	CMyVector();
	~CMyVector();
	void SetData();
	void GetData();
};

