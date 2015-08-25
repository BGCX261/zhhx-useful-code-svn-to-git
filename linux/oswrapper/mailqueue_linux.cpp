
#include <fcntl.h>
#include "OsError.h"
#include "OsWrapper.h"


int mailqueue_create(const char* name, MQHANDLE* phRecv, MQHANDLE* phSend, int nMaxMsgSize, int nMaxMsgNum)
{
	if( NULL==name || NULL==phRecv || NULL==phSend || nMaxMsgSize < 1)
	{
		return ERROR_INVALID_PARAM;
	}

	int nRet = ERROR_OSAPI_FAIL;
	
	struct mq_attr mqAttr;
	memset(&mqAttr, 0, sizeof(mqAttr));
	mqAttr.mq_maxmsg = nMaxMsgNum;
	mqAttr.mq_msgsize = nMaxMsgSize;

	int mq = mq_open(name, O_RDWR|O_CREAT|O_EXCL, S_IRWXU|S_IRWXG|S_IRWXO, &mqAttr);
	if( mq > -1 )
	{
		nRet = ERROR_NO_ERROR;
		*phSend = mq;
		*phRecv = mq;
	}

	return nRet;
}


int mailqueue_send(MQHANDLE handle, const void* pMsg, int nMsglen, int* pByteSent)
{
	if( handle < 0 || NULL==pMsg)
	{
		return ERROR_INVALID_PARAM;
	}

	int nRet = ERROR_OSAPI_FAIL;

	int nSents = mq_send(handle, pMsg, nMsglen, 1);
	if( nSents >= 0)
	{
		nRet = ERROR_NO_ERROR;
		if( pByteSent )
		{
			*pByteSent = nSents;
		}
	}

	return nRet;
}


int mailqueue_recv(MQHANDLE handle, void* pMsg, int nMsgBufLen, int* pBytesRecvd)
{
	if( handle < 0 || NULL == pMsg || nMsgBufLen < 1)
	{
		return ERROR_INVALID_PARAM;
	}

	int nRet = ERROR_OSAPI_FAIL;

	int nRes = mq_receive(handle, pMsg, nMsgBufLen, NULL);
	if( nRes >= 0)
	{
		nRet = ERROR_NO_ERROR;
		if( pBytesRecvd)
		{
			*pBytesRecvd = nRes;
		}
	}
	
	return nRet;
}


int mailqueue_close(MQHANDLE handle)
{
	int nRes = mq_close(handle);

	return ERROR_NO_ERROR;
}


int mailqueue_unlink(const char* name)
{
	if( NULL==name)
	{
		return ERROR_INVALID_PARAM;
	}

	int nRet = mq_unlink(name);
	if( nRet == 0 )
	{
		return ERROR_NO_ERROR;
	}
	else
	{
		return ERROR_OSAPI_FAIL;
	}
}

