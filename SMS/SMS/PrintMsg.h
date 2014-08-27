#pragma once
class CPrintMsg
{
public:
	CPrintMsg();
	~CPrintMsg();
	static void SetMachineStatus(SelfCheckInfo& stCheckInfo);
	static void SetIcCardMsg(CardInfoRead& stCardInfo);
};

