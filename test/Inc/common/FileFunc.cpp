#include "stdafx.h"
#include "FileFunc.h"

CFileFunc::CFileFunc()
{
}

CFileFunc::~CFileFunc()
{
}

CFileFunc& CFileFunc::Instance()
{
	static CFileFunc inst;
	return inst;
}

//////////////////////////////////////////////////////////////////////////
//
