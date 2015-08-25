
#include "stdafx.h"
#include "ParseMpeg2Ts.h"
#include "DecodeMpeg2ts.h"



CParseMpeg2ts::CParseMpeg2ts()
:	m_pcDecodeTs(NULL),
	m_pfFrameCallback(NULL),
	m_pUserData(NULL)
{

}


CParseMpeg2ts::~CParseMpeg2ts()
{
	if( m_pcDecodeTs)
	{
		delete m_pcDecodeTs;
		m_pcDecodeTs = NULL;
	}
}


int CParseMpeg2ts::Init(Mpeg2FrameCallback pfFrameCallback, void* userData)
{
	if( m_pcDecodeTs )
	{
		delete m_pcDecodeTs;
		m_pcDecodeTs = NULL;
	}

	m_pcDecodeTs = new CDecoderTs;
	m_pcDecodeTs->Init(pfFrameCallback, userData);

	m_pfFrameCallback = pfFrameCallback;
	m_pUserData = userData;

	return 0;
}


int CParseMpeg2ts::InputTsPackset(const unsigned char* pBuf, int nBufLen)
{
	if( m_pcDecodeTs )
	{
		return m_pcDecodeTs->DecodeTs(pBuf, nBufLen);
	}

	return 0;
}


int CParseMpeg2ts::Release()
{
	return 0;
}
