

#ifndef _TEST_DEFAULT_H_
#define	_TEST_DEFAULT_H_


#ifdef __cplusplus
#define		TESTDEFAULT_EXTERN		extern "C"
#else
#define		TESTDEFAULT_EXTERN
#endif //__cplusplus


#ifdef TESTDEFAULT_EXPORTS
#define	TESTDEFAULT_API  __declspec(dllexport)
#else
#define	TESTDEFAULT_API  __declspec(dllimport)
#endif //MINGMSVCDLL_EXPORTS



TESTDEFAULT_EXTERN TESTDEFAULT_API int TestDefaultFunc(int c);



#endif //_TEST_DEFAULT_H_
