
#ifdef WIN32
#include "stdafx.h"

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <msacm.h>

WAVEFORMATEX wfx;

#define MAX_IN_BUFFERS 32
#define USED_IN_BUFFERS 6
#define MAX_OUT_BUFFERS 32
#define USED_OUT_BUFFERS 32


unsigned int waveoutDeviceID = WAVE_MAPPER;
WAVEHDR waveHdrOut[MAX_OUT_BUFFERS];
HWAVEOUT hWaveOut;
char dataBufferOut[MAX_OUT_BUFFERS][3200];

unsigned int waveinDeviceID = WAVE_MAPPER;
HWAVEIN hWaveIn;
WAVEHDR waveHdrIn[MAX_IN_BUFFERS];
char dataBufferIn[MAX_IN_BUFFERS][3200];


int playbackwave(LPSTR pData, DWORD dwLen);


static void CALLBACK
SpeakerCallback (HWAVEOUT _hWaveOut, UINT uMsg, DWORD dwInstance,
                 DWORD dwParam1, DWORD dwParam2)
{
  WAVEHDR *wHdr;

  switch (uMsg)
    {
      case WOM_OPEN:
        fprintf (stderr, "SpeakerCallback : WOM_OPEN\n");
        break;
      case WOM_CLOSE:
        fprintf (stderr, "SpeakerCallback : WOM_CLOSE\n");
        break;
      case WOM_DONE:
        fprintf (stderr, "SpeakerCallback : WOM_DONE\n");
        wHdr = (WAVEHDR *) dwParam1;

        break;
      default:
        break;
    }
}


static void CALLBACK
WaveInCallback (HWAVEIN hWaveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1,
                DWORD dwParam2)
{
  WAVEHDR *wHdr;
  MMRESULT mr = NOERROR;

  switch (uMsg)
    {
      case WIM_OPEN:

        break;
      case WIM_CLOSE:

        break;
      case WIM_DATA:
        wHdr = (WAVEHDR *) dwParam1;

        mr = waveInAddBuffer (hWaveIn, &(waveHdrIn[wHdr->dwUser]),
                           sizeof (waveHdrIn[wHdr->dwUser]));
        if (mr != MMSYSERR_NOERROR)
        {
        } 
		else
        {
			playbackwave(wHdr->lpData, wHdr->dwBytesRecorded );
		}
        break;
      default:
        break;
    }
}

LPGSM610WAVEFORMAT gsmformat;

int g_g723Drivers = 0;
int g_gsmDrivers = 0;

BOOL CALLBACK
acmDriverEnumCallback (HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport)
{
  MMRESULT mmr;
  HACMDRIVER driver;
  int i;

  ACMDRIVERDETAILS details;

  details.cbStruct = sizeof (ACMDRIVERDETAILS);
  mmr = acmDriverDetails (hadid, &details, 0);

  mmr = acmDriverOpen (&driver, hadid, 0);

  for (i = 0; i < details.cFormatTags; i++)
    {
      ACMFORMATTAGDETAILS fmtDetails;

      ZeroMemory (&fmtDetails, sizeof (fmtDetails));
      fmtDetails.cbStruct = sizeof (ACMFORMATTAGDETAILS);
      fmtDetails.dwFormatTagIndex = i;
      mmr = acmFormatTagDetails (driver, &fmtDetails, ACM_FORMATTAGDETAILSF_INDEX);
      if (fdwSupport & ACMDRIVERDETAILS_SUPPORTF_CODEC)
        {
          /* fprintf(stderr, "supported codec: %s\n" ,details.szLongName ); */
          if (fmtDetails.dwFormatTag == WAVE_FORMAT_MSG723)
            {
              g_g723Drivers = /* 1 */ 0;
/*	!		  fprintf(stderr, "Find Codec  %s\n" ,details.szLongName ); */
#if 0
              fprintf (stderr, "G723 cFormatTags %i\n", details.cFormatTags);
              fprintf (stderr, "G723 cFilterTags %i\n", details.cFilterTags);
#endif
              return -1;
          } else if (fmtDetails.dwFormatTag == WAVE_FORMAT_GSM610)
            {
              g_gsmDrivers = 1;
/* !			  fprintf(stderr, "Find Codec  %s\n" ,details.szLongName ); */
              return -1;
            }
      } else
        {
/* !		  fprintf(stderr, "Unsupported codec: %s\n" ,details.szLongName ); */
        }
    }
  mmr = acmDriverClose (driver, 0);
  return -1;
}

int
os_sound_init ()
{
  g_g723Drivers = 0;
  g_gsmDrivers = 0;
  acmDriverEnum (acmDriverEnumCallback, 0, ACM_DRIVERENUMF_DISABLED);

  return 0;
}


int
open_sndcard (int format)
{
  MMRESULT mr = NOERROR;

  switch (format)
    {
      case WAVE_FORMAT_MULAW:
        wfx.wFormatTag = WAVE_FORMAT_MULAW;
        wfx.cbSize = 0;
        wfx.nAvgBytesPerSec = 8000;
        wfx.nBlockAlign = 1;
        wfx.nChannels = 1;
        wfx.nSamplesPerSec = 8000;
        wfx.wBitsPerSample = 8;
        break;
      case WAVE_FORMAT_ALAW:
        wfx.wFormatTag = WAVE_FORMAT_ALAW;
        wfx.cbSize = 0;
        wfx.nAvgBytesPerSec = 8000;
        wfx.nBlockAlign = 1;
        wfx.nChannels = 1;
        wfx.nSamplesPerSec = 8000;
        wfx.wBitsPerSample = 8;
        break;
      case WAVE_FORMAT_GSM610:
        gsmformat =
          (LPGSM610WAVEFORMAT)GlobalAlloc (GMEM_MOVEABLE, (UINT) (sizeof (GSM610WAVEFORMAT)));
        gsmformat = (LPGSM610WAVEFORMAT) GlobalLock (gsmformat);

        gsmformat->wfx.wFormatTag = WAVE_FORMAT_GSM610;
        gsmformat->wfx.nChannels = 1;
        gsmformat->wfx.nSamplesPerSec = 8000;
        gsmformat->wfx.nAvgBytesPerSec = 1625;
        gsmformat->wfx.nBlockAlign = 65;
        gsmformat->wfx.wBitsPerSample = 0;
        gsmformat->wfx.cbSize = 2;
        gsmformat->wSamplesPerBlock = 320;
        break;
      case WAVE_FORMAT_MSG723:
        wfx.wFormatTag = WAVE_FORMAT_MSG723;
        wfx.cbSize = 0;
        wfx.nAvgBytesPerSec = 800;
        wfx.nBlockAlign = 1;
        wfx.nChannels = 1;
        wfx.nSamplesPerSec = 8000;
        wfx.wBitsPerSample = 8;
        break;
        /*
           case WAVE_FORMAT_PCM:
           wfx.wFormatTag = WAVE_FORMAT_PCM;
           wfx.cbSize = 0;
           wfx.nAvgBytesPerSec = 16000;
           wfx.nBlockAlign = 2;
           wfx.nChannels = 1;
           wfx.nSamplesPerSec = 8000;
           wfx.wBitsPerSample = 16;
           break;
         */
      default:
        break;
    }

  if (gsmformat != NULL)
    mr =
      waveOutOpen (&hWaveOut, waveoutDeviceID,
                   (LPWAVEFORMATEX) & (gsmformat->wfx),
                   (DWORD) 0 /* SpeakerCallback */ , 0 /* arg */ ,
                   CALLBACK_NULL /* CALLBACK_FUNCTION */ );
  else
    mr =
      waveOutOpen (&hWaveOut, waveoutDeviceID, &wfx,
                   (DWORD) 0 /* SpeakerCallback */ , 0 /* arg */ ,
                   CALLBACK_NULL /* CALLBACK_FUNCTION */ );
  if (mr != NOERROR)
    {
/* !		fprintf(stderr, "__call_free: waveOutOpen: 0x%i\n", mr); */
      exit (-1);
      return -1;
  } else
    {
      int i;

      for (i = 0; i < USED_OUT_BUFFERS; i++)
        {
          memset (&(waveHdrOut[i]), 0, sizeof (waveHdrOut[i]));
          waveHdrOut[i].lpData = dataBufferOut[i];
          if (format == WAVE_FORMAT_GSM610)
            waveHdrOut[i].dwBufferLength = 65;  /* frameSize */
          else
            waveHdrOut[i].dwBufferLength = 320; /* frameSize */
          waveHdrOut[i].dwFlags = 0;
          waveHdrOut[i].dwUser = i;

          mr =
            waveOutPrepareHeader (hWaveOut, &(waveHdrOut[i]),
                                  sizeof (waveHdrOut[i]));
          if (mr != MMSYSERR_NOERROR)
            {
/* !				fprintf(stderr, "__call_free: waveOutPrepareHeader: 0x%i\n", mr); */
              exit (-1);
          } else
            {
            }
        }
    }

  if (gsmformat != NULL)
    mr =
      waveInOpen (&hWaveIn, waveinDeviceID,
                  (LPWAVEFORMATEX) & (gsmformat->wfx), (DWORD) WaveInCallback,
                  0, CALLBACK_FUNCTION);
  else
    mr =
      waveInOpen (&hWaveIn, waveinDeviceID, &wfx, (DWORD) WaveInCallback, 0,
                  CALLBACK_FUNCTION);
  if (mr != MMSYSERR_NOERROR)
    {
/* !		fprintf(stderr, "__call_free: waveInOpen: 0x%i\n", mr); */
      exit (-1);
      return -1;
  } else
    {
      int i;

      for (i = 0; i < MAX_IN_BUFFERS; i++)
        {
          waveHdrIn[i].lpData = dataBufferIn[i];
          waveHdrIn[i].dwBufferLength = 320;    /* frameSize */
          waveHdrIn[i].dwFlags = 0;
          waveHdrIn[i].dwUser = i;
          mr =
            waveInPrepareHeader (hWaveIn, &(waveHdrIn[i]), sizeof (waveHdrIn[i]));
          if (mr == MMSYSERR_NOERROR)
            {
              mr =
                waveInAddBuffer (hWaveIn, &(waveHdrIn[i]), sizeof (waveHdrIn[i]));
              if (mr == MMSYSERR_NOERROR)
                {
/* !					fprintf(stderr, "__call_free: waveInAddBuffer: 0x%i\n", mr); */
                  /* return -1; */
                }
          } else
            {
/* !				fprintf(stderr, "__call_free: waveInPrepareHeader: 0x%i\n", mr); */
              exit (-1);
              return -1;
            }
        }
      mr = waveInStart (hWaveIn);
      if (mr != MMSYSERR_NOERROR)
        {
/* !			fprintf(stderr, "__call_free: waveInStart: 0x%i\n", mr); */
          exit (-1);
          return -1;
        }
    }
  return 0;
}


int playbackwave(LPSTR pData, DWORD dwLen)
{
	static int pos_whdr = 0;
	static int sound_played = 0;
	static int cpt = 0;


	MMRESULT mr = NOERROR;

	memset (waveHdrOut[pos_whdr].lpData, 0, dwLen);
	memcpy (waveHdrOut[pos_whdr].lpData, pData, dwLen);
	waveHdrOut[pos_whdr].dwBufferLength = dwLen;
	mr =
		waveOutWrite (hWaveOut, &waveHdrOut[pos_whdr],
		sizeof (waveHdrOut[pos_whdr]));

	pos_whdr++;
	if (pos_whdr == USED_OUT_BUFFERS)
	{
		pos_whdr = 0;     /* loop over the prepared blocks */
	}
	if (mr != MMSYSERR_NOERROR)
		switch (mr)
	{
		case MMSYSERR_INVALHANDLE:
			fprintf (stderr,
				"__call_free: waveOutWrite: 0x%i MMSYSERR_INVALHANDLE\n",
				mr);
			break;
		case MMSYSERR_NODRIVER:
			fprintf (stderr,
				"__call_free: waveOutWrite: 0x%i MMSYSERR_NODRIVER\n",
				mr);
			break;
		case MMSYSERR_NOMEM:
			fprintf (stderr,
				"__call_free: waveOutWrite: 0x%i MMSYSERR_NOMEM\n", mr);
			break;
		case WAVERR_UNPREPARED:
			fprintf (stderr,
				"__call_free: waveOutWrite: 0x%i WAVERR_UNPREPARED\n",
				mr);
			break;
		case WAVERR_STILLPLAYING:
			fprintf (stderr,
				"__call_free: waveOutWrite: 0x%i WAVERR_STILLPLAYING\n",
				mr);
		default:
			fprintf (stderr, "__call_free: waveOutWrite: 0x%i\n", mr);
	} else
		++sound_played;
	fprintf (stderr, "sound played = %i\n", sound_played);
	fprintf (stderr, "cpt = %i\n", ++cpt);

	return 0;
}


int
os_sound_start ()
{
  int format = WAVE_FORMAT_MULAW;  
/*
  if (ca->payload == 0)
    format = WAVE_FORMAT_MULAW;
  else if (ca->payload == 8)
    format = WAVE_FORMAT_ALAW;
  else if (ca->payload == 18)
    format = WAVE_FORMAT_G729A;
  else if (ca->payload == 4)
    format = WAVE_FORMAT_GSM610; // WAVE_FORMAT_MSG72 ;
  else if (ca->payload == 3)
    format = WAVE_FORMAT_GSM610;
  else
    return -1;                  
*/
  if (open_sndcard (format) != 0)
    return -1; 
 	
  return 0;
}

void
os_sound_close ()
{ 
  waveInReset (hWaveIn);
  waveInClose (hWaveIn);
  waveOutReset (hWaveOut);
  waveOutClose (hWaveOut);
}

#endif
