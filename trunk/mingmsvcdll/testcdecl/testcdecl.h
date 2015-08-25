
#ifndef _TEST_CDECL_H_
#define _TEST_CDECL_H_

#ifdef __cplusplus
#define		TESTCDECLDLL_EXTERN		extern "C"
#else
#define		TESTCDECLDLL_EXTERN
#endif //__cplusplus


#ifdef TESTCDECL_EXPORTS
#define	TESTCDECL_API  __declspec(dllexport)
#else
#define	TESTCDECL_API  __declspec(dllimport)
#endif //MINGMSVCDLL_EXPORTS


TESTCDECLDLL_EXTERN TESTCDECL_API int __cdecl TestCdeclFunc(int a);



#endif //_TEST_CDECL_H_
