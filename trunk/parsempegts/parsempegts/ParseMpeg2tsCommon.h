
#pragma once


typedef enum
{
	ENCODE_TYPE_NONE,
	ENCODE_TYPE_H264,
	ENCODE_TYPE_MPEG4,
	ENCODE_TYPE_MPEG2
}EEncodeType;

typedef enum
{
	FRAME_TYPE_NONE,
	FRAME_TYPE_I_FRAME,
	FRAME_TYPE_P_FRAME
}EFrameType;


typedef struct tagFrameHeader
{
	EEncodeType eEncodeType;
	EFrameType eFrameType;
	unsigned int uFrameId;
	UINT64 llTimestamp;
	unsigned char* pFrameBuf;
	int nFrameLen;
}TFrameHeader;

typedef void (*Mpeg2FrameCallback)(TFrameHeader* ptFrameHeader, void* userData);


