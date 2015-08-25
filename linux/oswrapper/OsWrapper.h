#ifndef _MY_TYPES_H_
#define	_MY_TYPES_H_


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



//function call specification, and 64bit types
#if defined( WIN32 )
	#define __STDCALL  __stdcall
	#define __CDECL    __cdecl
	#define __INT64    __int64
	#define __UINT64    unsigned __int64
#else
	#define __STDCALL
	#define __CDECL
	#define __INT64    long long
	#define __UINT64   unsigned long long
#endif //WIN32


#if defined(LINUX32)
	
	#include <pthread.h>
	#include <sys/types.h>
	#include <semaphore.h>
	#include <sys/time.h>
	#include <sys/socket.h>
	#include <sys/select.h>

	#define ENABLE_COUNTER          100
	#define DISABLE_COUNTER         101
	#define GET_TSC_LOW             102
	#define GET_TSC_HIGH            103

	#define _ALIGN_(X) __attribute__ ((aligned(X)))
	#define	_PACK_	__attribute__ ((packed))

	#define CONST_LL(X) X##LL
	#define CONST_ULL(X) X##ULL

#elif defined(WIN32)

	#include <windows.h>

#else
#endif //LINUX32

typedef struct TEvent
{
#if defined(LINUX32)
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    int manual;
    int state;
#elif defined(WIN32)
	HANDLE handle;
#else
#endif //LINUX32
}TEvent;


typedef struct TMutex
{   
#if defined(LINUX32)
	pthread_mutex_t handle;
    int is_valid;
#elif defined(WIN32)
	CRITICAL_SECTION sCritSection;
    int iInited;
#else
#endif //LINUX32
}TMutex;


typedef struct TSemaphore
{    
#if defined(LINUX32)
	pthread_cond_t cond;
    pthread_mutex_t mutex;
    int count;
	int maxCount;
#elif defined(WIN32)
	HANDLE handle;
#else
#endif //LINUX32
}TSemaphore;


typedef struct TThread
{  
#if defined(LINUX32)
	pthread_t handle;
    int is_valid;
    void* (*p_thread_func)(void *);
    void *p_arg;
    TEvent exit_event;
    TMutex access_mut;
    int i_wait_count;
    int selected_cpu;
#elif defined(WIN32)
	HANDLE handle;
    TMutex access_mut;
    int i_wait_count;
    void* (__stdcall * protected_func)(void *);
    void *protected_arg;
    DWORD preset_affinity_mask;
    int selected_cpu;
#else
#endif //LINUX32
}TThread;

void event_set_invalid(TEvent *event);
int event_is_valid(TEvent *event);
int event_init(TEvent *event, int manual, int state);
int event_signal(TEvent *event);
int event_reset(TEvent *event);
int event_pulse(TEvent *event);
int event_wait(TEvent *event);
int event_timed_wait(TEvent *event, unsigned int msec);
void event_destroy(TEvent *event);


void semaphore_set_invalid(TSemaphore *sem);
int semaphore_is_valid(TSemaphore *sem);
int semaphore_init(TSemaphore *sem, int init_count, int maxCount);
int semaphore_timedwait(TSemaphore *sem, unsigned int msec);
int semaphore_wait(TSemaphore *sem);
int semaphore_try_wait(TSemaphore *sem);
int semaphore_post(TSemaphore *sem);
int semaphore_reset(TSemaphore* sem);
void semaphore_destroy(TSemaphore *sem);

/*
void mutex_set_invalid(TMutex *mutex);
int  mutex_is_valid(TMutex *mutex);
*/
int mutex_init(TMutex *mutex);
int mutex_lock(TMutex *mutex);
int mutex_unlock(TMutex *mutex);
int mutex_try_lock(TMutex *mutex);
void mutex_destroy(TMutex *mutex);


#if defined(WIN32)
#define OS_THREAD_CALLCONVENTION __stdcall
#elif defined(LINUX32)
#define OS_THREAD_CALLCONVENTION
#else
#define OS_THREAD_CALLCONVENTION
#endif

typedef void* (OS_THREAD_CALLCONVENTION * thread_callback)(void *);


#if defined(LINUX32)
typedef enum
{
	OS_THREAD_PRIORITY_HIGHEST,
	OS_THREAD_PRIORITY_HIGH,
	OS_THREAD_PRIORITY_NORMAL,
	OS_THREAD_PRIORITY_LOW,
	OS_THREAD_PRIORITY_LOWEST
} EThreadPriority;
#elif defined(WIN32)
typedef enum
{
	OS_THREAD_PRIORITY_LOWEST = THREAD_PRIORITY_LOWEST,
	OS_THREAD_PRIORITY_LOW = THREAD_PRIORITY_BELOW_NORMAL,
	OS_THREAD_PRIORITY_NORMAL = THREAD_PRIORITY_NORMAL,
	OS_THREAD_PRIORITY_HIGH = THREAD_PRIORITY_ABOVE_NORMAL,
	OS_THREAD_PRIORITY_HIGHEST = THREAD_PRIORITY_HIGHEST
} EThreadPriority;
#else
#endif //LINUX32




bool thread_is_valid(TThread *thread);
int thread_create(TThread *thread, thread_callback func, void *arg);
void thread_wait(TThread *thread);
int thread_timedwait(TThread *thread, unsigned int msec);
int  thread_set_priority(TThread *thread, EThreadPriority priority);
void thread_close(TThread *thread);
EThreadPriority get_current_thread_priority(void);
void set_current_thread_priority(EThreadPriority priority);
int get_thread_priority(TThread* thread, EThreadPriority* priority);
void thread_exit(TThread *thread, void* code);

#if defined(LINUX32)
typedef int MQHANDLE;	
#elif defined(WIN32)
typedef HANDLE MQHANDLE;
#else
#endif //LINUX32


int mailqueue_create(const char* name, MQHANDLE* phRecv, MQHANDLE* phSend, int nMaxMsgSize, int nMaxMsgNum);
int mailqueue_send(MQHANDLE handle, const void* pMsg, int nMsglen, int* pByteSent);
int mailqueue_recv(MQHANDLE handle, void* pMsg, int nMsgBufLen, int* pBytesRecvd);
int mailqueue_close(MQHANDLE handle);
int mailqueue_unlink(const char* name);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_MY_TYPES_H_
