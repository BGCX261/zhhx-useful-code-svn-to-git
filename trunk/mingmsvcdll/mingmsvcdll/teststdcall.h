

#ifndef _TEST_STDCALL_H_
#define	_TEST_STDCALL_H_



#ifdef __cplusplus
#define		TESTDLL_EXTERN		extern "C"
#else
#define		TESTDLL_EXTERN
#endif //__cplusplus

#ifdef MINGMSVCDLL_EXPORTS
#define	TESTDLL_API  __declspec(dllexport)
#else
#define	TESTDLL_API  __declspec(dllimport)
#endif //MINGMSVCDLL_EXPORTS



TESTDLL_EXTERN TESTDLL_API int __stdcall TestStdcallFunc(int b);



#endif //_TEST_STDCALL_H_
