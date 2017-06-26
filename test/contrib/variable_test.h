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
	void			function1(const int   nVar);		//���ݹ����Ĳ����ں����ڲ����Ըı�
	void			function2(const char* pVar);		//����ָ����ָ����Ϊ�������ɱ�
	void			function3(char* const pVar);		//����ָ�뱾��Ϊ�������ɱ�
	void			function4(const int&  nVar);		//���ò����ں����ڲ����Ըı�

private:
		
};

#endif