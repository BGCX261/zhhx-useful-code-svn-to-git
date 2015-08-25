#ifdef LINUX32

#include <sys/time.h>
#include <errno.h>
#ifndef __USE_UNIX98
	#define __USE_UNIX98
#endif
#include <pthread.h>
#include "OsError.h"
#include "OsWrapper.h"


void mutex_set_invalid(TMutex *mutex)
{
    if (NULL == mutex)
	{
        return;
	}

    mutex->is_valid = 0;
}


int mutex_is_valid(TMutex *mutex)
{
    if (NULL == mutex)
	{
        return 0;
	}

    return mutex->is_valid;
}


int mutex_init(TMutex *mutex)
{
    int nRet;    

    if (NULL == mutex)
	{
        return ERROR_NULL_PTR;
	}

    mutex->is_valid = !pthread_mutex_init(&mutex->handle, NULL);

	nRet = mutex->is_valid ? ERROR_NO_ERROR : ERROR_OSAPI_FAIL;
    
    return nRet;
}


int mutex_lock(TMutex *mutex)
{
    int nRet = ERROR_NOT_INIT;

    if (NULL == mutex)
	{
        return ERROR_NULL_PTR;
	}

    if (mutex->is_valid)
    {
        if (0 == pthread_mutex_lock(&mutex->handle))
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


int mutex_unlock(TMutex *mutex)
{
    int nRet = ERROR_NOT_INIT;

    if (NULL == mutex)
	{
        return ERROR_NULL_PTR;
	}

    if (mutex->is_valid)
    {
        if (0 == pthread_mutex_unlock(&mutex->handle))
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


int mutex_try_lock(TMutex *mutex)
{
    int nRet = ERROR_NOT_INIT;

    if (NULL == mutex)
	{
        return ERROR_NULL_PTR;
	}

    if (mutex->is_valid)
    {
        int iRes = pthread_mutex_trylock(&mutex->handle);
        switch (iRes)
        {
        case 0:
            nRet = ERROR_NO_ERROR;
            break;

        case EBUSY:
            nRet = ERROR_TIMEOUT;
            break;

        default:
            nRet = ERROR_OSAPI_FAIL;
            break;
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

    if(mutex->is_valid)
    {
        pthread_mutex_destroy(&mutex->handle);

        mutex->is_valid = 0;
    }
}
#else
# pragma warning( disable: 4206 )
#endif //LINUX32
