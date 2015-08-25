

#pragma once

#include "ParseMpeg2tsCommon.h"
#include <map>


#define		MAX_PMT_NUM_PER_PID				3
#define		MAX_ES_NUM_PER_PMT				4
#define		MAX_PID_INFO_NUM				(MAX_PMT_NUM_PER_PID*(MAX_ES_NUM_PER_PMT+1)+1)
#define		NORMAL_TS_PACKET_LEN			188
#define		NORMAL_FRAME_LEN				(256<<10)


typedef struct  
{
	int nStreamType;
	int nPid;
}TEs;

typedef struct 
{
	int nProramNumber;
	int nVersion;
	bool bCurrentNext;
	int nPcrPid;
	TEs atEs[MAX_ES_NUM_PER_PMT];
	int nEsNum;
}TPmt;

typedef struct
{
	unsigned short sPid;
	unsigned short sProgramNumber;
}TPatPmt;


typedef struct
{
	int nTransportStreamId;
	unsigned char nVersion;
	bool bCurrentNext;
	TPatPmt atProgram[MAX_PMT_NUM_PER_PID];
	int nProgramNum;
}TPat;



typedef struct  
{
	int nStreamType;
	int nPid;
	EEncodeType eEncodeId;
	int nTsPacketLost;
	UINT64 llTimestamp;
	int nFrameSize;
	unsigned char* pFrameBuf;
	int nFrameBufLen;
	int nFrameLen;
	unsigned int uFrameId;
	bool bSeenUnitStartFlag;
}TEsInfo;


typedef struct 
{
	//dvbpsi_handle hPmtHandle;
	int nVersion;
	int nProgramNumber;
	int nPcrPid;
	int nPid;
	TEsInfo atEs[MAX_ES_NUM_PER_PMT];
	int nEsNum;
}TPmtInfo;


typedef struct 
{
	//dvbpsi_handle hPatHandle;
	int nPatVersion;
	int nSdtVersion;
	TPmtInfo atProgram[MAX_PMT_NUM_PER_PID];
	int nProgramNum;
}TPatInfo;



typedef struct
{
	int nPid;
	bool bSeen;
	bool bValid;
	bool bPsi;
	bool bUsed;
	int nContinuityCounter;
	TPmtInfo* pPmtInfo;
	TEsInfo* ptEsInfo;
}TPidInfo;


class CDecoderTs
{
public:
	CDecoderTs();
	~CDecoderTs();

public:

	int Init(Mpeg2FrameCallback pfFrameCallback, void* userData);

	int DecodeTs(const unsigned char* pBuf, int nLen);

private:
	int GetPid(const unsigned char* pTsPacket, int nTsPacketLen) { return ((pTsPacket[1]&0x1f) << 8) |pTsPacket[2]; }
	//void HandlePATCallback(dvbpsi_pat_t* pNewPat);
	//void HandlePMTCallback(dvbpsi_pmt_t* pPmt);

	void HandlePATCallback();
	void HandlePMTCallback();

	TPidInfo* InitPidInfo(int pid, bool bPsi );
	int AnalyseTsPasket(const unsigned char* pBuf, int nLen);
	int GatherPes(TPidInfo*  ptPidInfo, const unsigned char* pBuf, int nLen);

	int CheckReadyCallback(TEsInfo* ptEsInfo, bool bSeeUnitStartFlag);

	int AnalysePat(const unsigned char* pBuf, int nLen);
	int AnalysePmt(const unsigned char* pBuf, int nLen);
	//static void PATCallback(void* userData, dvbpsi_pat_t* pNewPat);
	//static void PMtCallback(void* userData, dvbpsi_pmt_t* pPmt);

private:
	TPidInfo m_atPidInfo[MAX_PID_INFO_NUM];
	TPatInfo m_tPatInfo;
	unsigned char	m_pRemainderBuf[NORMAL_TS_PACKET_LEN];
	int		m_nRemainderDataLen;
	std::map<int, TPidInfo*> m_pidMap;

	TPat m_tPat;
	TPmt m_tPmt;

	Mpeg2FrameCallback m_pfFrameCallback;
	void* m_pUserData;
	TFrameHeader m_tFrameHeader;
};
