

#ifndef _TEST_CLASS_H_
#define _TEST_CLASS_H_


#ifdef __cplusplus
#define		TESTCLASS_EXTERN		extern "C"
#else
#define		TESTCLASS_EXTERN
#endif //__cplusplus

#ifdef TESTCLASS_EXPORTS
#define	TESTCLASS_API  __declspec(dllexport)
#else
#define	TESTCLASS_API  __declspec(dllimport)
#endif //



class TESTCLASS_API CTestClass
{
public:
	CTestClass();
	CTestClass(int a, int b);
	virtual ~CTestClass();

public:
	int SetValue(int a, int b);
	void GetValue(int& a, int& b);

private:
	int m_a;
	int m_b;
};



#endif //_TEST_CLASS_H_
