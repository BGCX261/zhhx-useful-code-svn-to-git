
#if defined(WIN32)

#include "OsError.h"
#include "OsWrapper.h"
#include "limits.h"


void semaphore_set_invalid(TSemaphore *sem)
{
    if (NULL == sem)
	{
        return;
	}

    sem->handle = NULL;
}


int semaphore_is_valid(TSemaphore *sem)
{
    if (NULL == sem)
	{
        return 0;
	}

    return (int)(0 != sem->handle);
}


int semaphore_init(TSemaphore *sem, int count, int maxCount)
{
    if (NULL == sem)
	{
        return ERROR_NULL_PTR;
	}

    sem->handle = CreateSemaphore(NULL, count, maxCount, 0);

    return (NULL != sem->handle) ? ERROR_NO_ERROR : ERROR_OSAPI_FAIL;
}


int semaphore_timedwait(TSemaphore *sem, unsigned int msec)
{
    int nRet = ERROR_NOT_INIT;
	
	if(sem && sem->handle)
    {
		nRet = ERROR_NO_ERROR;

        DWORD dwRes = WaitForSingleObject(sem->handle, msec);
		if (WAIT_TIMEOUT == dwRes)
		{
			nRet = ERROR_TIMEOUT;
		}
        else if (WAIT_OBJECT_0 != dwRes)
		{
            nRet = ERROR_OSAPI_FAIL;
		}
    }	

    return nRet;
}


int semaphore_wait(TSemaphore *sem)
{
    int nRet = ERROR_NOT_INIT;

    if (NULL == sem)
	{
        return ERROR_NULL_PTR;
	}

    if (sem->handle)
    {
        nRet = ERROR_NO_ERROR;
        if (WAIT_OBJECT_0 != WaitForSingleObject(sem->handle, INFINITE))
		{
            nRet = ERROR_OSAPI_FAIL;
		}
    }

    return nRet;
}


int semaphore_try_wait(TSemaphore *sem)
{
    if (NULL == sem)
	{
        return ERROR_NULL_PTR;
	}

    return semaphore_timedwait(sem, 0);
}


int semaphore_post(TSemaphore *sem)
{
    int nRet = ERROR_NOT_INIT;

    if (NULL == sem)
	{
        return ERROR_NULL_PTR;
	}

    if (sem->handle)
    {
        if (ReleaseSemaphore(sem->handle, 1, NULL))
		{
            nRet = ERROR_NO_ERROR;
		}
        else
		{
            nRet = ERROR_OSAPI_FAIL;
		}
    }

    return nRet;

}


int semaphore_reset(TSemaphore* sem)
{
	int nRet = ERROR_NOT_INIT;

	if( sem && sem->handle )
	{
		nRet = NO_ERROR;

		while( WAIT_OBJECT_0 == WaitForSingleObject(sem->handle, 0) )
		{

		}
	}

	return nRet;
}


void semaphore_destroy(TSemaphore *sem)
{
    if (NULL == sem)
	{
        return;
	}

    if (sem->handle)
    {
        CloseHandle(sem->handle);
        sem->handle = NULL;
    }
}

#endif //defined(WIN32)
