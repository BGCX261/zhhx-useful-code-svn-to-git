
#if defined(WIN32)

#include "OsError.h"
#include "OsWrapper.h"


void event_set_invalid(TEvent *event)
{
	if (NULL == event)
	{
        return;
	}

    event->handle = NULL;
}


int event_is_valid(TEvent *event)
{
    if (NULL == event)
	{
        return 0;
	}

    return (int)(NULL != event->handle);
}


int event_init(TEvent *event, int manual, int state)
{
	int nRet = ERROR_NO_ERROR;

    if (NULL == event)
        return ERROR_NULL_PTR;

    event_destroy(event);
    event->handle = CreateEvent(NULL, manual, state, NULL);

    if (NULL == event->handle)
	{
        nRet = ERROR_OSAPI_FAIL;
	}
    else
	{		
        nRet =  ERROR_NO_ERROR;
	}

	return nRet;
}


int event_signal(TEvent *event)
{
    if (NULL == event)
	{
        return ERROR_NULL_PTR;
	}

	int nRet = ERROR_NO_ERROR;

    if (NULL == event->handle)
	{
        nRet = ERROR_NOT_INIT;
	}
    else if (SetEvent(event->handle))
	{
        nRet = ERROR_NO_ERROR;
	}
    else
	{
        nRet = ERROR_OSAPI_FAIL;
	}

	return nRet;
}


int event_reset(TEvent *event)
{
    if (NULL == event)
	{
        return ERROR_NULL_PTR;
	}

	int nRet = ERROR_NO_ERROR;

    if (NULL == event->handle)
	{
        nRet = ERROR_NOT_INIT;
	}
    else if (ResetEvent(event->handle))
	{
        nRet = ERROR_NO_ERROR;
	}
    else
	{
        nRet =  ERROR_OSAPI_FAIL;
	}

	return nRet;
}


int event_pulse(TEvent *event)
{
    if (NULL == event)
	{
        return ERROR_NULL_PTR;
	}

	int nRet = ERROR_NO_ERROR;

    if (NULL == event->handle)
	{
         nRet = ERROR_NOT_INIT;
	}
    else if (PulseEvent(event->handle))
	{
        nRet =  ERROR_NO_ERROR;
	}
    else
	{
        nRet =  ERROR_OSAPI_FAIL;
	}

	return nRet;
}


int event_wait(TEvent *event)
{
    if (NULL == event)
	{
        return ERROR_NULL_PTR;
	}

	int nRet = ERROR_NO_ERROR;

    if (NULL == event->handle)
	{
        nRet = ERROR_NOT_INIT;
	}
    else if (WAIT_OBJECT_0 == WaitForSingleObject(event->handle, INFINITE))
	{
        nRet = ERROR_NO_ERROR;
	}
    else
	{
        nRet =  ERROR_OSAPI_FAIL;
	}

	return nRet;
}


int event_timed_wait(TEvent *event, unsigned int msec)
{
    if (NULL == event)
	{
        return ERROR_NULL_PTR;
	}

	int nRet = ERROR_NO_ERROR;

    if (NULL != event->handle)
    {
        unsigned int dwRes = WaitForSingleObject(event->handle, msec);

        if (WAIT_OBJECT_0 == dwRes)
		{
			nRet = ERROR_NO_ERROR;
		}
        else if (WAIT_TIMEOUT == dwRes)
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
		nRet = ERROR_NOT_INIT;
	}

    return nRet;
}


void event_destroy(TEvent *event)
{
    if (NULL == event)
	{
        return;
	}

    if (event->handle)
    {
        CloseHandle(event->handle);
        event->handle = NULL;
    }
}

#endif //defined(WIN32)
