#ifdef LINUX32

#include <sys/time.h>
#include <pthread.h>
#include <errno.h>
#include "TEvent.h"


void event_set_invalid(TEvent *event)
{
    if (NULL == event)
	{
        return;
	}

    event->state= -1;
}


int event_is_valid(TEvent *event)
{

    if (NULL == event)
	{
        return 0;
	}

    return event->state >= 0;
}


int event_init(TEvent *event, int manual, int state)
{
    if (NULL == event)
	{
        return ERROR_NULL_PTR;
	}

    event->manual = manual;
    event->state = state ? 1 : 0;

    pthread_cond_init(&event->cond, 0);

    pthread_mutex_init(&event->mutex, 0);

    return ERROR_NO_ERROR;
}


int event_signal(TEvent *event)
{
    int nRet = ERROR_NOT_INIT;

    if (NULL == event)
	{
        return ERROR_NULL_PTR;
	}

    if (0 <= event->state)
    {
        pthread_mutex_lock(&event->mutex);

        if (0 == event->state)
        {
            event->state = 1;

            if (event->manual)
			{
                pthread_cond_broadcast(&event->cond);
			}
            else
			{
                pthread_cond_signal(&event->cond);
			}
        }

        nRet = ERROR_NO_ERROR;

        pthread_mutex_unlock(&event->mutex);
    }

    return nRet;
}

int event_reset(TEvent *event)
{
    int nRet = ERROR_NOT_INIT;

	if (NULL == event)
	{
        return ERROR_NULL_PTR;
	}

    if (0 <= event->state)
    {
        pthread_mutex_lock(&event->mutex);

        if (1 == event->state)
		{
            event->state = 0;
		}

        pthread_mutex_unlock(&event->mutex);
        
		nRet = ERROR_NO_ERROR;
    }

    return nRet;
}


int event_pulse(TEvent *event)
{
    int nRet = ERROR_NOT_INIT;

    if (NULL == event)
	{
        return ERROR_NULL_PTR;
	}

    if (0 <= event->state)
    {
        pthread_mutex_lock(&event->mutex);

        if (event->manual)
		{
            pthread_cond_broadcast(&event->cond);
		}
        else
		{
            pthread_cond_signal(&event->cond);
		}

        event->state = 0;

        pthread_mutex_unlock(&event->mutex);

        nRet = ERROR_NO_ERROR;
    }

    return nRet;
}


int event_wait(TEvent *event)
{
    int nRet = ERROR_NOT_INIT;

    if (NULL == event)
	{
        return ERROR_NULL_PTR;
	}

    if (0 <= event->state)
    {
        pthread_mutex_lock(&event->mutex);

        if (!event->state)
		{
            pthread_cond_wait(&event->cond, &event->mutex);
		}

        if (!event->manual)
		{
            event->state = 0;
		}

        pthread_mutex_unlock(&event->mutex);

        nRet = ERROR_NO_ERROR;
    }

    return nRet;
}


int event_timed_wait(TEvent *event, unsigned int msec)
{
    int nRet = ERROR_NOT_INIT;

    if (NULL == event)
	{
        return ERROR_NULL_PTR;
	}

    if (0 <= event->state)
    {
        pthread_mutex_lock(&event->mutex);

        if (0 == event->state)
        {
            struct timeval tval;
            struct timespec tspec;
            int iRes;

            gettimeofday(&tval, NULL);
            msec = 1000 * msec + tval.tv_usec;
            tspec.tv_sec = tval.tv_sec + msec / 1000000;
            tspec.tv_nsec = (msec % 1000000) * 1000;
            iRes = pthread_cond_timedwait(&event->cond,
                                           &event->mutex,
                                           &tspec);
            if (0 == iRes)
			{
                nRet = ERROR_NO_ERROR;
			}
            else if (ETIMEDOUT == iRes)
			{
                nRet = ERROR_TIMEOUT;
			}
            else
			{
                nRet = ERROR_OSAPI_FAIL;
			}
        }
        else
		{
            nRet = ERROR_NO_ERROR;
		}

        if (!event->manual)
		{
            event->state = 0;
		}

        pthread_mutex_unlock(&event->mutex);
    }

    return nRet;
}


void event_destroy(TEvent *event)
{
    if (NULL == event)
	{
        return;
	}

    if (event->state >= 0)
    {
        pthread_cond_destroy(&event->cond);
        pthread_mutex_destroy(&event->mutex);
        
		event->state= -1;
    }
}

#else
# pragma warning( disable: 4206 )
#endif //LINUX32
