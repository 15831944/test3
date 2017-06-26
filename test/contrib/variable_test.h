#ifndef __variable_test_h__
#define __variable_test_h__

class CTestClass
{
public:
	void			func1()				{};
	int				func2() const		{ return m_nCount;};

public:
	const char*		GetString()	const	{m_nCount++; return (const char*)"this is a test!";};			

protected:
	enum {size1=100, size2=200};
	int				array1[size1];
	int				array2[size2];

	mutable	int		m_nCount;
};


class CVariableTest
{
public:
	CVariableTest();
	~CVariableTest();
	
public:
	void			Test_const();
	void			Test_volatile();
	void			Test_mutalbe();

protected:
	void			function1(const int   nVar);		//传递过来的参数在函数内不可以改变
	void			function2(const char* pVar);		//参数指针所指内容为常量不可变
	void			function3(char* const pVar);		//参数指针本身为常量不可变
	void			function4(const int&  nVar);		//引用参数在函数内不可以改变

private:
		
};

#endif