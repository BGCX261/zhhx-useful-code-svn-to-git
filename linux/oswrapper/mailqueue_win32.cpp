#include "OsError.h"
#include "OsWrapper.h"



int mailqueue_create(const char* name, MQHANDLE* phRecv, MQHANDLE* phSend, int nMaxMsgSize, int nMaxMsgNum)
{
	if( NULL == phSend  || NULL == phSend || nMaxMsgSize <= 0 )
	{
		return ERROR_INVALID_PARAM;
	}

	int nRet = ERROR_NO_ERROR;

	int nSize = nMaxMsgSize * nMaxMsgNum;

	BOOL bRet = CreatePipe(phRecv, phSend, NULL, nSize );
	if( !bRet )
	{
		bRet = ERROR_OSAPI_FAIL;
	}

	return nRet;
}


int mailqueue_send(MQHANDLE handle, const void* pMsg, int nMsglen, int* pByteSent)
{
	if( NULL==handle || NULL==pMsg )
	{
		return ERROR_INVALID_PARAM;
	}

	int nRet = ERROR_OSAPI_FAIL;
	DWORD dwBytesWrite = 0;
	BOOL bRet = WriteFile(handle, pMsg, nMsglen, &dwBytesWrite, NULL);
	if( bRet )
	{
		nRet = ERROR_NO_ERROR;		
	}

	if( pByteSent )
	{
		*pByteSent = dwBytesWrite;
	}

	return nRet;
}


int mailqueue_recv(MQHANDLE handle, void* pMsg, int nMsgBufLen, int* pBytesRecvd)
{
	if( NULL==handle || NULL==pMsg || nMsgBufLen <= 0)
	{
		return ERROR_INVALID_PARAM;
	}

	int nRet = ERROR_OSAPI_FAIL;
	DWORD dwBytesRead = 0;
	BOOL bRet = ReadFile(handle, pMsg, nMsgBufLen, &dwBytesRead, NULL);
	if( bRet )
	{
		nRet = ERROR_NO_ERROR;
	}
	if( pBytesRecvd )
	{
		*pBytesRecvd = dwBytesRead;
	}

	return nRet;
}


int mailqueue_close(MQHANDLE handle)
{
	if( handle )
	{
		CloseHandle(handle);
	}

	return ERROR_NO_ERROR;
}


int mailqueue_unlink(const char*)
{
	return ERROR_NO_ERROR;
}
