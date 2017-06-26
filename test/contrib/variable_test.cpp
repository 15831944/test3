#include "stdafx.h"
#include "variable_test.h"

CVariableTest::CVariableTest()
{
	
}

CVariableTest::~CVariableTest()
{
	
}

/*
	如果const位于*的左侧，则const就是用来修饰指针所指向的变量，即指针指向为常量;
	如果const位于*的右侧，const就是修饰指针本身，即指针本身是常量;
*/
void CVariableTest::Test_const()
{
	//const
	char* const s1 = "test1";			//指针本身是常量不可变
	const char *s2 = "test2";			//指针所指向的内容是常量不可变
	char const *s3 = "test3";			//...
	const char* const s4 = "test4";		//都不可变

	//
	const CTestClass constObj;
	constObj.func2();
}

void CVariableTest::Test_volatile()
{
	CString strInfo;

	//volatile
	volatile int a1 = 123;
	volatile int a2 = 10;
	int a3 = a2;
	strInfo.Format(_T("a2=%d"), a3); AfxMessageBox(strInfo);
	__asm{ 
		mov dword ptr [ebp-4], 20h 
	}
	int a4 = a2;
	strInfo.Format(_T("a2=%d"), a4); AfxMessageBox(strInfo);
}

void CVariableTest::Test_mutalbe()
{
	//mutalbe
	const CTestClass constObj;
	constObj.func2();
	constObj.GetString();
	constObj.func2();
}