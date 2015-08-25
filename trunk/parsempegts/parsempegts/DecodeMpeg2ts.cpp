#include "stdafx.h"
#include "DecodeMpeg2ts.h"
#include <assert.h>



CDecoderTs::CDecoderTs()
:	m_nRemainderDataLen(0),
	m_pfFrameCallback(NULL),
	m_pUserData(NULL)
{
	memset(m_atPidInfo, 0, sizeof(m_atPidInfo));
	memset(&m_tPatInfo, 0, sizeof(TPatInfo));

	int i=0;
	for(i=0; i<MAX_PMT_NUM_PER_PID; i++)
	{
		m_tPatInfo.atProgram[i].nVersion = -1;
		m_tPatInfo.atProgram[i].nProgramNumber = -1;
	}

	m_atPidInfo[0].bPsi = true;
	m_atPidInfo[0].bSeen = false;
	m_atPidInfo[0].bValid = true;
	m_atPidInfo[0].bUsed = true;
	m_atPidInfo[0].nContinuityCounter = -1;
	m_atPidInfo[0].nPid = 0;
	m_atPidInfo[0].ptEsInfo = NULL;
	

	m_pidMap.insert( std::pair<int, TPidInfo*>(0, &m_atPidInfo[0]) );
		 

	m_tPatInfo.nPatVersion = -1;
	m_tPatInfo.nSdtVersion = -1;
	//m_tPatInfo.hPatHandle = dvbpsi_AttachPAT(PATCallback, this);
}


CDecoderTs::~CDecoderTs()
{
	m_pidMap.clear();
	for(int i=0; i<MAX_PMT_NUM_PER_PID; i++)
	{
		for(int j=0; j<MAX_ES_NUM_PER_PMT; j++ )
		{
			if( m_tPatInfo.atProgram[i].atEs[j].pFrameBuf )
			{
				delete m_tPatInfo.atProgram[i].atEs[j].pFrameBuf;
				m_tPatInfo.atProgram[i].atEs[j].pFrameBuf = NULL;
			}
		}
	}
}


int CDecoderTs::Init(Mpeg2FrameCallback pfFrameCallback, void* userData)
{
	m_pfFrameCallback = pfFrameCallback;
	m_pUserData = userData;

	return 0;
}


int CDecoderTs::DecodeTs(const unsigned char* pBuf, int nLen)
{
	if( NULL == pBuf  || nLen < 1 )
	{
		return 0;
	}

	const unsigned char* pStart = NULL;
	int nDataLen = 0;

	if( m_nRemainderDataLen > 0 )
	{
		pStart = m_pRemainderBuf;
		nDataLen = m_nRemainderDataLen;
		memcpy(m_pRemainderBuf+m_nRemainderDataLen, pBuf, NORMAL_TS_PACKET_LEN-m_nRemainderDataLen);

		AnalyseTsPasket(m_pRemainderBuf, NORMAL_TS_PACKET_LEN);

		pBuf +=  NORMAL_TS_PACKET_LEN-m_nRemainderDataLen;
		nLen -=  NORMAL_TS_PACKET_LEN-m_nRemainderDataLen;

		m_nRemainderDataLen = 0;		
	}

	pStart = pBuf;
	while( pStart[0] != 0x47 && nLen )
	{
		pStart++;
		nLen--;
	}	
	
	while( nLen >= NORMAL_TS_PACKET_LEN )
	{
		AnalyseTsPasket(pStart, NORMAL_TS_PACKET_LEN);
		pStart += NORMAL_TS_PACKET_LEN;
		nLen -= NORMAL_TS_PACKET_LEN;
	}

	if( nLen > 0 )
	{
		memcpy(m_pRemainderBuf, pStart, nLen );
		m_nRemainderDataLen = nLen;
	}

	return 0;
}


int CDecoderTs::AnalyseTsPasket(const unsigned char* pBuf, int nLen)
{
	TPidInfo*  ptPidInfo = NULL;

	int nPid = GetPid(pBuf, nLen);
	std::map<int, TPidInfo*>::const_iterator it = m_pidMap.find(nPid);
	if( it != m_pidMap.end() )
	{
		ptPidInfo = it->second;
	}

	if( ptPidInfo == NULL || !ptPidInfo->bValid )
	{
		return 0;
	}

	if( ptPidInfo->bPsi )
	{
		if( ptPidInfo->nPid == 0 )
		{
			//dvbpsi_PushPacket(m_tPatInfo.hPatHandle, pBuf);
			AnalysePat(pBuf, nLen);
		}
		else
		{
			//dvbpsi_PushPacket(ptPidInfo->pPmtInfo->hPmtHandle, pBuf);
			AnalysePmt(pBuf, nLen);
				
		}
	}
	else
	{
		GatherPes(ptPidInfo, pBuf, nLen);
	}

	return 0;
}


int CDecoderTs::GatherPes(TPidInfo*  ptPidInfo, const unsigned char* pBuf, int nLen)
{
	bool bUnitStart = pBuf[1] & 0x40;
	bool bScrambled = pBuf[3] & 0xc0;
	bool bAdaptation = pBuf[3] & 0x20;
	bool bPayload = pBuf[3] & 0x10;
	int nContinuityCounter = pBuf[3] & 0x0f;
	bool bDiscontinuity = false;

	int nSkip = 0;
	if( !bAdaptation )
	{
		nSkip = 4;
	}
	else
	{
		nSkip = 5 + pBuf[4];
		if( pBuf[4] > 0 )
		{
			bDiscontinuity = pBuf[5] & 0x80;
		}
	}

	int nCcDiff = (nContinuityCounter - ptPidInfo->nContinuityCounter) & 0x0f;
	if( bPayload && nCcDiff == 1 )
	{
		ptPidInfo->nContinuityCounter = (ptPidInfo->nContinuityCounter + 1 ) & 0x0f;
	}
	else
	{
		if( ptPidInfo->nContinuityCounter == -1 )
		{
			ptPidInfo->nContinuityCounter = nContinuityCounter;
		}
		else
		{
			ptPidInfo->ptEsInfo->nTsPacketLost++;
		}
	}

	pBuf += nSkip;
	nLen -= nSkip;

	if( bUnitStart )
	{
		if( ptPidInfo->ptEsInfo->nFrameLen > 0 )
		{
			//callback a frame
			CheckReadyCallback(ptPidInfo->ptEsInfo, true);
		}

		//a new frame
		if(pBuf[0] != 0 || pBuf[1] != 0 || pBuf[2] != 1)
		{
			//
			return 0;
		}

		int nPesSize = (pBuf[4] << 8) + pBuf[5];
		bool bPts = pBuf[7] & 0x80;
		bool bDts = pBuf[7] & 0x40;
		int nPesSkip = pBuf[8] + 9;

		if( bPts )
		{
			UINT64 ulPts = ( (UINT64)(pBuf[9]&0x0e) << 29) |
				( (UINT64)(pBuf[10] << 22) )   |
				( (UINT64)(pBuf[11] &0xfe) << 14) |
				( (UINT64)(pBuf[12] << 7) ) |
				( (UINT64)(pBuf[13] >> 1));

			ptPidInfo->ptEsInfo->llTimestamp = ulPts / 90;//90kHz, convert to ms
		}

		ptPidInfo->ptEsInfo->nFrameSize = nPesSize>0 ? (nPesSize - pBuf[8] - 3) : 0;
		
		pBuf += nPesSkip;
		nLen -= nPesSkip;
		
		ptPidInfo->ptEsInfo->nTsPacketLost = 0;
		ptPidInfo->ptEsInfo->nFrameLen = 0;
		ptPidInfo->ptEsInfo->bSeenUnitStartFlag = true;
	}

	if( ptPidInfo->ptEsInfo->nFrameLen + nLen > ptPidInfo->ptEsInfo->nFrameBufLen )
	{
		delete ptPidInfo->ptEsInfo->pFrameBuf;
		ptPidInfo->ptEsInfo->nFrameBufLen = (ptPidInfo->ptEsInfo->nFrameLen + nLen) * 2;
		ptPidInfo->ptEsInfo->pFrameBuf = new unsigned char[ptPidInfo->ptEsInfo->nFrameBufLen];
	}

	memcpy(ptPidInfo->ptEsInfo->pFrameBuf+ptPidInfo->ptEsInfo->nFrameLen, pBuf, nLen);
	ptPidInfo->ptEsInfo->nFrameLen += nLen;

	if(ptPidInfo->ptEsInfo->nFrameLen > 0 && ptPidInfo->ptEsInfo->nFrameSize > 0 && ptPidInfo->ptEsInfo->nFrameLen == ptPidInfo->ptEsInfo->nFrameSize )
	{
		CheckReadyCallback(ptPidInfo->ptEsInfo, bUnitStart);
	}

	return 0;
}


int CDecoderTs::CheckReadyCallback(TEsInfo* ptEsInfo, bool bSeeUnitStartFlag)
{
	//callback a frame
	ptEsInfo->uFrameId++;
	if( ptEsInfo->nTsPacketLost == 0 && ptEsInfo->nFrameLen > 0 && ptEsInfo->bSeenUnitStartFlag )
	{
		m_tFrameHeader.eEncodeType = ptEsInfo->eEncodeId;
		m_tFrameHeader.pFrameBuf = ptEsInfo->pFrameBuf;
		m_tFrameHeader.nFrameLen = ptEsInfo->nFrameLen;
		m_tFrameHeader.llTimestamp = ptEsInfo->llTimestamp;
		m_tFrameHeader.uFrameId = ptEsInfo->uFrameId;

		if( m_pfFrameCallback )
		{
			m_pfFrameCallback(&m_tFrameHeader, m_pUserData);
		}
	}

	ptEsInfo->nTsPacketLost = 0;
	ptEsInfo->nFrameLen = 0;
	ptEsInfo->bSeenUnitStartFlag = false;
	ptEsInfo->nFrameSize = 0;

	return 0;
}


TPidInfo* CDecoderTs::InitPidInfo(int pid, bool bPsi )
{
	int i=0;
	for(i=1; i<MAX_PID_INFO_NUM; i++)
	{
		if( m_atPidInfo[i].bUsed == false )
		{
			break;
		}
	}

	if( i>= MAX_PID_INFO_NUM )
	{
		assert(false);
//		TRACE("error!!!!!!!!");

		return NULL;
	}
	else
	{
		m_pidMap.insert(std::pair<int, TPidInfo*>(pid, &m_atPidInfo[i]));
		m_atPidInfo[i].bPsi = bPsi;
		m_atPidInfo[i].bSeen = false;
		m_atPidInfo[i].bUsed = true;
		m_atPidInfo[i].bValid = true;
		m_atPidInfo[i].nContinuityCounter = -1;
		m_atPidInfo[i].nPid = pid;
		m_atPidInfo[i].ptEsInfo = NULL;
		m_atPidInfo[i].pPmtInfo = NULL;

		return &m_atPidInfo[i];
	}

	return NULL;
}




void CDecoderTs::HandlePATCallback()
{
	//已经回调过，而且没有变化或者下次pat才有效
	if( m_tPatInfo.nPatVersion != -1 &&
		(!m_tPat.bCurrentNext || m_tPat.nVersion == m_tPatInfo.nPatVersion ))
	{
		return;
	}

	m_tPatInfo.nProgramNum = 0;	

	for(int i=0; i<m_tPat.nProgramNum && m_tPatInfo.nProgramNum <MAX_PMT_NUM_PER_PID; i++)
	{
		if( m_tPat.atProgram[i].sProgramNumber != 0 )
		{
			m_tPatInfo.atProgram[m_tPatInfo.nProgramNum].nPid = m_tPat.atProgram[i].sPid;
			m_tPatInfo.atProgram[m_tPatInfo.nProgramNum].nProgramNumber = m_tPat.atProgram[i].sProgramNumber;			

			TPidInfo* ptPidInfo = InitPidInfo(m_tPat.atProgram[i].sPid, true);
			ptPidInfo->pPmtInfo = &m_tPatInfo.atProgram[m_tPatInfo.nProgramNum];

			m_tPatInfo.nProgramNum++;
		}
	}


	m_atPidInfo[0].bSeen = true;
	m_atPidInfo[0].bValid = true;
	m_tPatInfo.nPatVersion = m_tPat.nVersion;

	return;
}


void CDecoderTs::HandlePMTCallback()
{
	int i = 0;

	for( i=0; i<m_tPatInfo.nProgramNum; i++ )
	{
		if( m_tPatInfo.atProgram[i].nProgramNumber == m_tPmt.nProramNumber &&
			m_tPatInfo.atProgram[i].nVersion != -1 &&			
			(!m_tPmt.bCurrentNext || m_tPatInfo.atProgram[i].nVersion == m_tPmt.nVersion ))
		{			
			return;
		}
	}

	TPmtInfo* pShootPmt = NULL;
	for( i=0; i<m_tPatInfo.nProgramNum; i++ )
	{
		if( m_tPatInfo.atProgram[i].nProgramNumber == m_tPmt.nProramNumber )
		{
			pShootPmt = &m_tPatInfo.atProgram[i];
			break;
		}
	}

	if( i>= m_tPatInfo.nProgramNum )
	{
		if( m_tPatInfo.nProgramNum >= MAX_PMT_NUM_PER_PID )
		{			
			return;
		}

		pShootPmt = &m_tPatInfo.atProgram[m_tPatInfo.nProgramNum];
		m_tPatInfo.nProgramNum++;		
	}

	pShootPmt->nEsNum = 0;
	pShootPmt->nPcrPid = m_tPmt.nPcrPid;
	pShootPmt->nVersion = m_tPmt.nVersion;

	std::map<int, TPidInfo*>::const_iterator it = m_pidMap.find(pShootPmt->nPid);
	if( it != m_pidMap.end() )
	{
		it->second->bSeen = true;
	}


	for( i=0; i<m_tPmt.nEsNum && pShootPmt->nEsNum<MAX_ES_NUM_PER_PMT; i++ )
	{
		pShootPmt->atEs[pShootPmt->nEsNum].nStreamType = m_tPmt.atEs[i].nStreamType;
		pShootPmt->atEs[pShootPmt->nEsNum].nPid = m_tPmt.atEs[i].nPid;
		if( m_tPmt.atEs[i].nStreamType == 0x1b)
		{
			pShootPmt->atEs[pShootPmt->nEsNum].eEncodeId = ENCODE_TYPE_H264;
		}
		else if( m_tPmt.atEs[i].nStreamType == 0x10 )
		{
			pShootPmt->atEs[pShootPmt->nEsNum].eEncodeId = ENCODE_TYPE_MPEG4;
		}
		else if( m_tPmt.atEs[i].nStreamType == 0x02 )
		{
			pShootPmt->atEs[pShootPmt->nEsNum].eEncodeId = ENCODE_TYPE_MPEG2;
		}
		else
		{
			pShootPmt->atEs[pShootPmt->nEsNum].eEncodeId = ENCODE_TYPE_NONE;
		}

		if( pShootPmt->atEs[pShootPmt->nEsNum].pFrameBuf )
		{
			delete pShootPmt->atEs[pShootPmt->nEsNum].pFrameBuf;
		}
		pShootPmt->atEs[pShootPmt->nEsNum].pFrameBuf = new unsigned char[NORMAL_FRAME_LEN];
		pShootPmt->atEs[pShootPmt->nEsNum].nFrameBufLen = NORMAL_FRAME_LEN;
		TPidInfo* ptPidInfo = InitPidInfo(m_tPmt.atEs[i].nPid, false);
		if( ptPidInfo )
		{
			ptPidInfo->ptEsInfo = &pShootPmt->atEs[pShootPmt->nEsNum];
		}

		pShootPmt->nEsNum++;
	}

	return;
}


int CDecoderTs::AnalysePat(const unsigned char* pBuf, int nLen)
{
	memset(&m_tPat, 0, sizeof(m_tPat));
	bool bUnitStart = pBuf[1] & 0x40;
	bool bScrambled = pBuf[3] & 0xc0;
	bool bAdaptation = pBuf[3] & 0x20;
	bool bPayload = pBuf[3] & 0x10;
	int nContinuityCounter = pBuf[3] & 0x0f;
	bool bDiscontinuity = false;

	int nSkip = 0;
	if( !bAdaptation )
	{
		nSkip = 4;
	}
	else
	{
		nSkip = 5 + pBuf[4];
		if( pBuf[4] > 0 )
		{
			bDiscontinuity = pBuf[5] & 0x80;
		}
	}

	pBuf += nSkip;
	nLen -= nSkip;//ts packet header analysis end

	int nPointerLen = pBuf[0];
	pBuf += nPointerLen+1;
	nLen -= nPointerLen+1;

	//pat section analysis start
	int nTableId = pBuf[0];
	int nSectionLen = (((int)(pBuf[1])&0x0f)<<8) + pBuf[2];
	m_tPat.nTransportStreamId = (pBuf[3]<<8) + pBuf[4];
	m_tPat.nVersion = pBuf[5] & 0x3e;
	m_tPat.bCurrentNext  = pBuf[5] & 0x01;
	int nSectionNum = pBuf[6];
	int nLastSectionNum = pBuf[7];

	pBuf += 8;
	nLen -= 8;

	int nPmtLen = nSectionLen - 5 -4;
	m_tPat.nProgramNum = nPmtLen / 4;
	for(int i=0; i<m_tPat.nProgramNum && nPmtLen >= 4 && nLen >= 0; i++ )
	{
		m_tPat.atProgram[i].sProgramNumber = (pBuf[0]<<8) + pBuf[1];
		m_tPat.atProgram[i].sPid = ((pBuf[2]&0x1f)<<8) + pBuf[3];
		nPmtLen -= 4;
		pBuf += 4;
		nLen -= 4;
	}

	HandlePATCallback();

	return 0;
}


int CDecoderTs::AnalysePmt(const unsigned char* pBuf, int nLen)
{
	memset(&m_tPmt, 0, sizeof(m_tPmt));

	bool bUnitStart = pBuf[1] & 0x40;
	bool bScrambled = pBuf[3] & 0xc0;
	bool bAdaptation = pBuf[3] & 0x20;
	bool bPayload = pBuf[3] & 0x10;
	int nContinuityCounter = pBuf[3] & 0x0f;
	bool bDiscontinuity = false;

	int nSkip = 0;
	if( !bAdaptation )
	{
		nSkip = 4;
	}
	else
	{
		nSkip = 5 + pBuf[4];
		if( pBuf[4] > 0 )
		{
			bDiscontinuity = pBuf[5] & 0x80;
		}
	}

	pBuf += nSkip;
	nLen -= nSkip;//ts packet header analysis end

	int nPointerLen = pBuf[0];
	pBuf += nPointerLen+1;
	nLen -= nPointerLen+1;

	//pat section analysis start
	int nTableId = pBuf[0];
	int nSectionLen = (((int)(pBuf[1])&0x0f)<<8) + pBuf[2];
	m_tPmt.nProramNumber = (pBuf[3]<<8) + pBuf[4];
	m_tPmt.nVersion = pBuf[5] & 0x3e;
	m_tPmt.bCurrentNext  = pBuf[5] & 0x01;
	int nSectionNum = pBuf[6];
	int nLastSectionNum = pBuf[7];
	m_tPmt.nPcrPid = ((pBuf[8]&0x1f)<<8) + pBuf[9];

	int nProgramInfoLen = ((pBuf[10]&0x0f)<<8) + pBuf[11];

	pBuf += 12 + nProgramInfoLen;
	nLen -= 12 + nProgramInfoLen;

	int nEsLen = nSectionLen - 9 - 4 - nProgramInfoLen;
	m_tPmt.nEsNum = 0;
	for(int i=0; m_tPmt.nEsNum<MAX_ES_NUM_PER_PMT && nEsLen >= 5 && nLen >= 5; i++ )
	{
		m_tPmt.atEs[i].nStreamType = pBuf[0];
		m_tPmt.atEs[i].nPid = ((pBuf[1]&0x1f)<<8) + pBuf[2];
		int nEsInfoLen = ((pBuf[3]&0x0f)<<8) + pBuf[4];
		m_tPmt.nEsNum++;

		pBuf += 5+nEsInfoLen;
		nLen -= 5+nEsInfoLen;
		nEsLen -= 5+nEsInfoLen;
	}

	HandlePMTCallback();

	return 0;
}

/*
void CDecoderTs::PATCallback(void* userData, dvbpsi_pat_t* pNewPat)
{
	CDecoderTs* pDecoderTs = (CDecoderTs*)userData;

	if( pDecoderTs )
	{
	pDecoderTs->HandlePATCallback(pNewPat);
	}
}


void CDecoderTs::PMTCallback(void* userData, dvbpsi_pmt_t* pPmt)
{
	CDecoderTs* pDecoderTs = (CDecoderTs*)userData;

	if( pDecoderTs )
	{
	pDecoderTs->HandlePMTCallback(pPmt);
	}
}


void CDecoderTs::HandlePATCallback(dvbpsi_pat_t* pNewPat)
{
	//已经回调过，而且没有变化或者下次pat才有效
	if( m_tPatInfo.nPatVersion != -1 &&
	(!pNewPat->b_current_next || pNewPat->i_version == m_tPatInfo.nPatVersion ))
	{
		dvbpsi_DeletePAT(pNewPat);

	return;
}

dvbpsi_pat_program_t* pProgram=NULL;
m_tPatInfo.nProgramNum = 0;	

for(pProgram=pNewPat->p_first_program; pProgram!=NULL && m_tPatInfo.nProgramNum <MAX_PMT_NUM_PER_PID; pProgram=pProgram->p_next)
{
if( pProgram->i_number != 0 )
{
m_tPatInfo.atProgram[m_tPatInfo.nProgramNum].nPid = pProgram->i_pid;
m_tPatInfo.atProgram[m_tPatInfo.nProgramNum].nProgramNumber = pProgram->i_number;			
m_tPatInfo.atProgram[m_tPatInfo.nProgramNum].hPmtHandle = dvbpsi_AttachPMT(pProgram->i_number, PMTCallback, this);						

TPidInfo* ptPidInfo = InitPidInfo(pProgram->i_pid, true);
ptPidInfo->pPmtInfo = &m_tPatInfo.atProgram[m_tPatInfo.nProgramNum];

m_tPatInfo.nProgramNum++;
}
}


m_atPidInfo[0]->bSeen = true;
m_atPidInfo[0]->bValid = true;
m_tPatInfo.nPatVersion = pNewPat->i_version;
dvbpsi_DeletePAT(pNewPat);

return;
}


void CDecoderTs::HandlePMTCallback(dvbpsi_pmt_t* pPmt)
{
dvbpsi_descriptor_t* pDescriptor=NULL;
dvbpsi_pmt_es_t* pEs=NULL;

int i = 0;

for( i=0; i<m_tPatInfo.nProgramNum; i++ )
{
if( m_tPatInfo.atProgram[i].nProgramNumber == pPmt->i_program_number &&
m_tPatInfo.atProgram[i].nVersion != -1 &&			
!pPmt->b_current_next || m_tPatInfo.atProgram[i].nVersion == pPmt->i_version )
{
dvbpsi_DeletePMT(pPmt);
return;
}
}

TPmtInfo* pShootPmt = NULL;
for( i=0; i<m_tPatInfo.nProgramNum; i++ )
{
if( m_tPatInfo.atProgram[i].nProgramNumber == pPmt->i_program_number )
{
pShootPmt = &m_tPatInfo.atProgram[i];
break;
}
}

if( i>= m_tPatInfo.nProgramNum )
{
if( m_tPatInfo.nProgramNum >= MAX_PMT_NUM_PER_PID )
{
dvbpsi_DeletePMT(pPmt);
return;
}

pShootPmt = &m_tPatInfo.atProgram[m_tPatInfo.nProgramNum];
m_tPatInfo.nProgramNum++;		
}

pShootPmt->nEsNum = 0;
pShootPmt->nPcrPid = pPmt->i_pcr_pid;
pShootPmt->nVersion = pPmt->i_version;

std::map<int, TPidInfo*>::const_iterator it = m_pidMap.find(pShootPmt->nPid);
if( it != m_pidMap.end() )
{
it->second->bSeen = true;
}


for( pEs=pPmt->p_first_es; pEs!=NULL && pShootPmt->nEsNum<MAX_ES_NUM_PER_PMT; pEs=pEs->next )
{
pShootPmt->atEs[pShootPmt->nEsNum].nStreamType = pEs->i_type;
pShootPmt->atEs[pShootPmt->nEsNum].nPid = pEs->i_pid;
if( pShootPmt->atEs[pShootPmt->nEsNum].pFrameBuf )
{
delete pShootPmt->atEs[pShootPmt->nEsNum].pFrameBuf;
}
pShootPmt->atEs[pShootPmt->nEsNum].pFrameBuf = new unsigned char[NORMAL_FRAME_LEN];
pShootPmt->atEs[pShootPmt->nEsNum].nFrameBufLen = NORMAL_FRAME_LEN;
TPidInfo ptPidInfo = InitPidInfo(pEs->i_pid, false);
if( ptPidInfo )
{
ptPidInfo->ptEsInfo = &pShootPmt->atEs[pShootPmt->nEsNum];
}

pShootPmt->nEsNum++;
}

dvbpsi_DeletePMT(pPmt);
return;
}
*/