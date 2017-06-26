#include "stdafx.h"
#include "variable_test.h"

CVariableTest::CVariableTest()
{
	
}

CVariableTest::~CVariableTest()
{
	
}

/*
	���constλ��*����࣬��const������������ָ����ָ��ı�������ָ��ָ��Ϊ����;
	���constλ��*���Ҳ࣬const��������ָ�뱾����ָ�뱾���ǳ���;
*/
void CVariableTest::Test_const()
{
	//const
	char* const s1 = "test1";			//ָ�뱾���ǳ������ɱ�
	const char *s2 = "test2";			//ָ����ָ��������ǳ������ɱ�
	char const *s3 = "test3";			//...
	const char* const s4 = "test4";		//�����ɱ�

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