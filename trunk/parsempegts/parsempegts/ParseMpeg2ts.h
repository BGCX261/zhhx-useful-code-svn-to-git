

#pragma once




#include "ParseMpeg2tsCommon.h"

class CDecoderTs;

class CParseMpeg2ts
{
public:
	CParseMpeg2ts();
	~CParseMpeg2ts();

public:
	int Init(Mpeg2FrameCallback pfFrameCallback, void* userData);

	int InputTsPackset(const unsigned char* pBuf, int nBufLen);

	int Release();

private:
	CDecoderTs* m_pcDecodeTs;
	Mpeg2FrameCallback m_pfFrameCallback;
	void* m_pUserData;
};
