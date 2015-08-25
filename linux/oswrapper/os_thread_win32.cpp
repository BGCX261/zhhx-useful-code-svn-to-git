
#if defined(WIN32)

#include <stdlib.h>
#include "OsError.h"
#include "OsWrapper.h"


bool thread_is_valid(TThread *thread)
{
    bool bRet = false;

    if (NULL == thread)
	{
        return false;
	}

    mutex_lock(&thread->access_mut);
    if(NULL != thread->handle)
    {
        if (WAIT_OBJECT_0 == WaitForSingleObject(thread->handle, 0))//thread is terminated
        {
            bRet = false;
            if (0 == thread->i_wait_count)
            {
                CloseHandle(thread->handle);
                thread->handle = NULL;
            }
        }
		else
		{
			bRet = true;
		}
    }    
    
	mutex_unlock(&(thread->access_mut));

    return bRet;
}


int thread_create(TThread *thread, thread_callback func, void *arg)
{
    int nRet = ERROR_NO_ERROR;

    if( (NULL == thread) || (NULL == func) )
	{
        return ERROR_NULL_PTR;
	}   

    if( ERROR_NO_ERROR != (nRet = mutex_init(&thread->access_mut)) )
	{
        return nRet;
	}

    mutex_lock(&thread->access_mut);

	thread->protected_func = func;
	thread->protected_arg  = arg;

    thread->handle = (HANDLE)_beginthreadex(0, 0, (void* (__stdcall *)(void*))func, arg, 0, 0);

    nRet = (thread->handle) ? (ERROR_NO_ERROR) : (ERROR_OSAPI_FAIL);

    mutex_unlock(&thread->access_mut);

    thread->preset_affinity_mask = 0;
	thread->i_wait_count = 0;
	thread->selected_cpu = 0;

    return nRet;
}


int thread_set_priority(TThread *thread, EThreadPriority priority)
{
	int nRet = ERROR_NO_ERROR;    

    if (NULL == thread)
	{
        nRet = ERROR_NULL_PTR;
	}

	nRet = ERROR_NOT_INIT;

    if(NULL != thread->handle)
    {
        mutex_lock(&thread->access_mut);

        nRet = SetThreadPriority(thread->handle, priority);//nonzero when successful
		if( !nRet )
		{
			nRet = ERROR_OSAPI_FAIL;
		}

        mutex_unlock(&thread->access_mut);
    }

    return nRet;
}


void thread_wait(TThread *thread)
{
    if (NULL == thread)
	{
        return;
	}

    mutex_lock(&thread->access_mut);
    if(thread->handle)
    {
        thread->i_wait_count++;
        mutex_unlock(&thread->access_mut);

        WaitForSingleObject(thread->handle, INFINITE);

        mutex_lock(&thread->access_mut);
        thread->i_wait_count--;
        if(0 == thread->i_wait_count)
        {
            CloseHandle(thread->handle);
            thread->handle = NULL;
        }
    }
    mutex_unlock(&thread->access_mut);
}


int thread_timedwait(TThread *thread, unsigned int msec)
{
	int nRet = ERROR_NO_ERROR;

	if( NULL == thread || NULL == thread->handle )
	{
		return ERROR_NULL_PTR;
	}

	mutex_lock(&thread->access_mut);
	if(thread->handle)
	{
		thread->i_wait_count++;
		mutex_unlock(&thread->access_mut);

		DWORD dwRet = WaitForSingleObject(thread->handle, msec);
		if( dwRet == WAIT_TIMEOUT )
		{
			nRet = ERROR_TIMEOUT;
		}
		else if( dwRet == WAIT_OBJECT_0 )
		{
			nRet = ERROR_NO_ERROR;
		}
		else
		{
			nRet = ERROR_OSAPI_FAIL;
		}

		mutex_lock(&thread->access_mut);
		thread->i_wait_count--;
		if(0 == thread->i_wait_count)
		{
			CloseHandle(thread->handle);
			thread->handle = NULL;
		}
	}

	mutex_unlock(&thread->access_mut);

	return nRet;
}


void thread_close(TThread *thread)
{
    if (NULL == thread)
        return;

    thread_wait(thread);

    mutex_destroy(&thread->access_mut);
}


void thread_exit(TThread *thread, void* code)
{
	if( NULL== thread )
	{
		return;
	}

	ExitThread((DWORD)code);
}


EThreadPriority get_current_thread_priority()
{
    return (EThreadPriority)GetThreadPriority(GetCurrentThread());
}


void set_current_thread_priority(EThreadPriority priority)
{
    SetThreadPriority(GetCurrentThread(), (int)priority);
}


int set_thread_priority(TThread* thread, EThreadPriority priority)
{
	int nRet = ERROR_NO_ERROR;

	if( NULL == thread )
	{
		nRet = ERROR_NULL_PTR;
	}

	if( NULL == thread->handle )
	{
		nRet = ERROR_NOT_INIT;
	}
	else
	{
		BOOL bRet = SetThreadPriority(thread->handle, priority);
		if( !bRet )
		{
			nRet = ERROR_OSAPI_FAIL;
		}		
	}

	return nRet;	
}


int get_thread_priority(TThread* thread, EThreadPriority* priority)
{
	int nRet = ERROR_NO_ERROR;

	if( NULL == thread || NULL == priority)
	{
		nRet = ERROR_NULL_PTR;
	}

	if( NULL == thread->handle )
	{
		nRet = ERROR_NOT_INIT;
	}
	else
	{
		int sRet = GetThreadPriority(thread->handle);
		if( sRet == THREAD_PRIORITY_ERROR_RETURN )
		{
			nRet = ERROR_OSAPI_FAIL;
		}
		else
		{
			*priority = sRet;
		}
	}

	return nRet;
}

#endif //defined(WIN32)
