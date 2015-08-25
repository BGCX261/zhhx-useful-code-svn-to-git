// parsempegts.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "parsempegts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "ParseMpeg2ts.h"

FILE* pfH264=NULL;
FILE* pfMetaData=NULL;


void Mpeg2FrameCb(TFrameHeader* ptFrameHeader, void* userData)
{
	if( ptFrameHeader->eEncodeType == ENCODE_TYPE_H264 )
	{
		fwrite(ptFrameHeader->pFrameBuf, 1, ptFrameHeader->nFrameLen, pfH264);
	}
	else if(ptFrameHeader->eEncodeType == ENCODE_TYPE_METADATA )
	{
		fwrite(ptFrameHeader->pFrameBuf, 1, ptFrameHeader->nFrameLen, pfMetaData);
	}
}


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		pfH264=fopen("h264.h264", "wb");
		pfMetaData=fopen("metadata.log", "wb");

		CParseMpeg2ts* pcParseMpegts = new CParseMpeg2ts;
		pcParseMpegts->Init(Mpeg2FrameCb, 0);

#define	READ_LEN		188*10+7
		unsigned char achBuf[READ_LEN];
		FILE* pfTsfile = fopen("testts5.ts", "rb");
		int nReadNum = fread(achBuf, 1, READ_LEN, pfTsfile);
		while( nReadNum )
		{
			pcParseMpegts->InputTsPackset(achBuf, nReadNum);

			nReadNum = fread(achBuf, 1, READ_LEN, pfTsfile);
		}

		fclose(pfH264);
		fclose(pfMetaData);
		fclose(pfTsfile);
		delete pcParseMpegts;
	}

	return nRetCode;
}
