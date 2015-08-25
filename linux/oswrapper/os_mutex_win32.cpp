

#if defined(WIN32)

#include "OsError.h"
#include "OsWrapper.h"

/*
void mutex_set_invalid(TMutex *mutex)
{
	if (NULL == mutex)
	{
        return;
	}

    memset(&mutex->sCritSection, 0, sizeof(CRITICAL_SECTION));
    mutex->iInited = 0;
}


Ipp32s mutex_is_valid(TMutex *mutex)
{
    if (NULL == mutex)
        return 0;

    return mutex->iInited;

}
*/

int mutex_init(TMutex *mutex)
{
    if (NULL == mutex)
	{
        return ERROR_NULL_PTR;
	}

    mutex_destroy(mutex);

    InitializeCriticalSection(&mutex->sCritSection);
    
	mutex->iInited = 1;

    return ERROR_NO_ERROR;
}


int mutex_lock(TMutex *mutex)
{
    int nRet = ERROR_NO_ERROR;

    if (NULL == mutex)
	{
        return ERROR_NULL_PTR;
	}

    if (mutex->iInited)
    {
		EnterCriticalSection(&mutex->sCritSection);
    }
    else
	{
        nRet = ERROR_NOT_INIT;
	}

    return nRet;

}


int mutex_unlock(TMutex *mutex)
{
    if (NULL == mutex)
	{
        return ERROR_NULL_PTR;
	}

    if (mutex->iInited)
    {
        LeaveCriticalSection(&mutex->sCritSection);
        return ERROR_NO_ERROR;
    }
    else
	{
        return ERROR_NOT_INIT;
	}
}


int mutex_try_lock(TMutex *mutex)
{
	if (NULL == mutex)
	{
		return ERROR_NULL_PTR;
	}

	int nRet = ERROR_NOT_INIT;

	if (mutex->iInited)
	{
		nRet = ERROR_NO_ERROR;

		if(! TryEnterCriticalSection(&mutex->sCritSection))
		{
			nRet = ERROR_OSAPI_FAIL;
		}
	}

	return nRet;
}


void mutex_destroy(TMutex *mutex)
{
     if (NULL == mutex)
	 {
        return;
	 }

    if(mutex->iInited)
    {
        mutex->iInited = 0;
        DeleteCriticalSection(&mutex->sCritSection);
	}
}

#endif // defined(WIN32)
