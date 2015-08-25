#ifdef LINUX32

#include <unistd.h>
#include <sys/time.h>
#include <sched.h>
#include "OsError.h"
#include "OsWrapper.h"


static void *thread_proc(void *pv_params)
{
    TThread *p_thread = (TThread *) pv_params;

    if (NULL == pv_params)
	{
        return ((void *) ERROR_NULL_PTR);
	}

    p_thread->p_thread_func(p_thread->p_arg);

    event_signal(&p_thread->exit_event);

    return ((void *) ERROR_NO_ERROR);
}


void thread_set_invalid(TThread *thread)
{
    if (NULL == thread)
	{
        return;
	}

    thread->is_valid = 0;
    thread->i_wait_count = 0;

    event_set_invalid(&thread->exit_event);

    //mutex_set_invalid(&thread->access_mut);
}


int thread_is_valid(TThread *thread)
{
    if (NULL == thread)
	{
        return 0;
	}

	int nRet = 0;

    if (thread->is_valid)
    {
        mutex_lock(&thread->access_mut);

        if (ERROR_NO_ERROR == event_timed_wait(&thread->exit_event, 0))
        {
            mutex_unlock(&thread->access_mut);
            
			thread_wait(thread);
        }
        else
		{
            mutex_unlock(&thread->access_mut);
		}
    }

    return thread->is_valid;
}


int thread_create(TThread *thread, void* (*thread_func)(void *), void *arg)
{
    int nRet = ERROR_NO_ERROR;

    pthread_attr_t attr;

    if ((NULL == thread) || (NULL == thread_func) )
	{
        return ERROR_INVALID_PARAM;
	}

	if (ERROR_NO_ERROR != event_init(&thread->exit_event, 1, 0))
	{
        return ERROR_OSAPI_FAIL;
	}    

    if(ERROR_NO_ERROR != mutex_init(&thread->access_mut))
	{
		return ERROR_OSAPI_FAIL;
	}
    
    {
        mutex_lock(&thread->access_mut);
        
		thread->p_thread_func = thread_func;
        thread->p_arg = arg;
        pthread_attr_init(&attr);

        pthread_attr_setschedpolicy(&attr, geteuid() ? SCHED_OTHER : SCHED_RR);//root or users

        thread->is_valid =! pthread_create(&thread->handle, &attr,thread_proc, (void*)thread);

        nRet = (thread->is_valid) ? ERROR_NO_ERROR : ERROR_OSAPI_FAIL;

        mutex_unlock(&thread->access_mut);

        pthread_attr_destroy(&attr);
    }

    //thread_set_priority(thread, VM_THREAD_PRIORITY_LOWEST);

    return nRet;
}


int thread_set_priority(TThread *thread, EThreadPriority priority)
{
    int nRet = ERROR_NO_ERROR;

    int policy, pmin, pmax, pmean;
	
	struct sched_param param;

    if (NULL == thread)
	{
        return ERROR_NULL_PTR;
	}

    if (thread->is_valid)
    {
        mutex_lock(&thread->access_mut);

        pthread_getschedparam(thread->handle, &policy, &param);

        pmin = sched_get_priority_min(policy);

        pmax = sched_get_priority_max(policy);

        pmean = (pmin + pmax) / 2;

        switch (priority)
        {
        case VM_THREAD_PRIORITY_HIGHEST:
            param.sched_priority = pmax;
            break;

        case VM_THREAD_PRIORITY_LOWEST:
            param.sched_priority = pmin;
            break;

        case VM_THREAD_PRIORITY_NORMAL:
            param.sched_priority = pmean;
            break;

        case VM_THREAD_PRIORITY_HIGH:
            param.sched_priority = (pmax + pmean) / 2;
            break;

        case VM_THREAD_PRIORITY_LOW:
            param.sched_priority = (pmin + pmean) / 2;
            break;

        default:
            nRet = ERROR_OSAPI_FAIL;
            break;
        }

        if (ERROR_NO_ERROR == nRet)
		{
            int nRes = pthread_setschedparam(thread->handle, policy, &param);
			if( nRes != 0 )
			{
				nRet = ERROR_OSAPI_FAIL;
			}
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

    if (thread->is_valid)
    {
        mutex_lock(&thread->access_mut);
        thread->i_wait_count++;
        mutex_unlock(&thread->access_mut);

        event_wait(&thread->exit_event);

        mutex_lock(&thread->access_mut);
        thread->i_wait_count--;
        if (0 == thread->i_wait_count)
        {
            pthread_join(thread->handle, NULL);
            thread->is_valid = 0;
        }
        mutex_unlock(&thread->access_mut);
    }
}


int thread_timedwait(TThread *thread, unsigned int msec)
{
	if (NULL == thread)
	{
		return;
	}

	int nRet = ERROR_NO_ERROR;

	if (thread->is_valid)
	{
		mutex_lock(&thread->access_mut);
		thread->i_wait_count++;
		mutex_unlock(&thread->access_mut);

		nRet = event_timed_wait(&thread->exit_event, msec);

		mutex_lock(&thread->access_mut);
		thread->i_wait_count--;
		if (0 == thread->i_wait_count)
		{
			pthread_join(thread->handle, NULL);
			thread->is_valid = 0;
		}
		mutex_unlock(&thread->access_mut);
	}
}


void thread_close(TThread *thread)
{
    if (NULL == thread)
        return;

    thread_wait(thread);
    event_destroy(&thread->exit_event);
    mutex_destroy(&thread->access_mut);
}


void thread_exit(TThread *thread, void* code)
{
	if( NULL == thread  || !thread->is_valid)
	{
		return;
	}

	pthread_exit(code);
}


EThreadPriority vm_get_current_thread_priority()
{
    return VM_THREAD_PRIORITY_NORMAL;
}


void vm_set_current_thread_priority(EThreadPriority priority)
{
    priority = priority;
}


int get_thread_priority(TThread* thread, EThreadPriority* priority)
{
	int nRet = ERROR_NO_ERROR;

	int policy, pmin, pmax, pmean;

	struct sched_param param;

	if (NULL == thread)
	{
		return ERROR_NULL_PTR;
	}

	if (thread->is_valid)
	{
		mutex_lock(&thread->access_mut);

		pthread_getschedparam(thread->handle, &policy, &param);

		pmin = sched_get_priority_min(policy);

		pmax = sched_get_priority_max(policy);

		pmean = (pmin + pmax) / 2;

		if( param.sched_priority == pmax )
		{
			*priority = OS_THREAD_PRIORITY_HIGHEST;
		}
		else if(param.sched_priority == pmin)
		{
			*priority = VM_THREAD_PRIORITY_LOWEST;
		}
		else if(param.sched_priority == pmean)
		{
			*priority = VM_THREAD_PRIORITY_NORMAL;
		}
		else if( param.sched_priority == (pmax + pmean) / 2)
		{
			*priority = VM_THREAD_PRIORITY_HIGH
		}
		else if(param.sched_priority == (pmin + pmean) / 2)
		{
			*priority = VM_THREAD_PRIORITY_LOW;
		}
		else
		{
			*priority = VM_THREAD_PRIORITY_NORMAL;
		}

		mutex_unlock(&thread->access_mut);
	}

	return nRet;
}

#else
# pragma warning( disable: 4206 )
#endif //LINUX32
