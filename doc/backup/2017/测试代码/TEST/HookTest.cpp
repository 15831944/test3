#include "stdafx.h"
#include "HookTest.h"

HOOKTest::HOOKTest()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DWORD nErr = DetourTransactionCommit();
}

HOOKTest::~HOOKTest()
{
}