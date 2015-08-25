
#ifdef LINUX32

#include <semaphore.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include "OsError.h"
#include "OsWrapper.h"


void semaphore_set_invalid(TSemaphore *sem)
{
    if (NULL == sem)
	{
        return;
	}

    sem->count = -1;
	sem->maxCount = -1;
}


int semaphore_is_valid(TSemaphore *sem)
{
    if (NULL == sem)
	{
        return 0;
	}

    return (-1 < sem->maxCount);
}



int semaphore_init(TSemaphore *sem, int init_count, int maxCount)
{
    if (NULL == sem)
	{
        return ERROR_NULL_PTR;
	}

	if( init_count < 0 || maxCount < 0 || init_count > maxCount )
	{
		return ERROR_INVALID_PARAM;
	}

    sem->count = init_count;
	sem->maxCount = maxCount;

    pthread_cond_init(&sem->cond, NULL);

    pthread_mutex_init(&sem->mutex,NULL);

    return ERROR_NO_ERROR;

}


int semaphore_timedwait(TSemaphore *sem, unsigned int msec)
{
    int nRet = ERROR_NOT_INIT;

    if (NULL == sem)
	{
        return ERROR_NULL_PTR;
	}

    if (0 < sem->maxCount)
    {
        nRet = ERROR_NO_ERROR;

        pthread_mutex_lock(&sem->mutex);

        if (0 == sem->count)
        {
            struct timeval tval;
            struct timespec tspec;
            int  iRes;

            gettimeofday(&tval, NULL);
            msec = 1000 * msec + tval.tv_usec;
            tspec.tv_sec = tval.tv_sec + msec / 1000000;
            tspec.tv_nsec = (msec % 1000000) * 1000;
            iRes = pthread_cond_timedwait(&sem->cond, &sem->mutex, &tspec);

            if (ETIMEDOUT == iRes)
			{
                nRet = ERROR_TIMEOUT;
			}
            else if (0 != iRes)
			{
                nRet = ERROR_OSAPI_FAIL;
			}
        }

        if (ERROR_NO_ERROR == nRet)
		{
			assert(sem->count > 0);
            sem->count--;
		}

        pthread_mutex_unlock(&sem->mutex);
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

    if (0 < sem->maxCount)
    {
        nRet = ERROR_NO_ERROR;

        pthread_mutex_lock(&sem->mutex);

        if (0 == sem->count && 0 != pthread_cond_wait(&sem->cond, &sem->mutex))
		{
            nRet = ERROR_OSAPI_FAIL;
		}

        if (ERROR_NO_ERROR == nRet)
		{
			assert(sem->count > 0);
            sem->count--;
		}

        pthread_mutex_unlock(&sem->mutex);
    }

    return nRet;
}


int semaphore_try_wait(TSemaphore *sem)
{
    int nRet = ERROR_NOT_INIT;

    if (NULL == sem)
	{
        return ERROR_NULL_PTR;
	}

    if (0 < sem->maxCount)
    {
        pthread_mutex_lock(&sem->mutex);

        if (0 == sem->count)
		{
            nRet = ERROR_TIMEOUT;
		}
        else
        {
			assert(sem->count > 0);
            sem->count--;			
            nRet = ERROR_NO_ERROR;
        }

        pthread_mutex_unlock(&sem->mutex);
    }

    return nRet;
}


int semaphore_post(TSemaphore *sem)
{
    int nRet = ERROR_NOT_INIT;

    if (NULL == sem)
	{
        return ERROR_NULL_PTR;
	}

    if (0 < sem->maxCount )
    {
        pthread_mutex_lock(&sem->mutex);

		sem->count++;
		if( sem->count > sem->maxCount )
		{
			sem->count = sem->maxCount;
		}
		if( 1 == sem->count )
		{
			pthread_cond_signal(&sem->cond);
		}

        pthread_mutex_unlock(&sem->mutex);
        nRet = ERROR_NO_ERROR;
    }
    return nRet;

}


void semaphore_destroy(TSemaphore *sem)
{
    if (NULL == sem)
	{
        return;
	}

    if (0 < sem->maxCount)
    {
        pthread_cond_destroy(&sem->cond);
        pthread_mutex_destroy(&sem->mutex);
        sem->count = -1;
		sem->maxCount = -1;
    }
}

#else
# pragma warning( disable: 4206 )
#endif //LINUX32
