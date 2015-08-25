// freetype.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "freetype.h"
#include <stdio.h>
#include <string.h>
#include "testfreetype.h"
#include <iconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

#include <stdio.h>
#include <stdlib.h>
# include <windows.h>
#include <string.h>
# include <shlobj.h>

# define HAVE_STYLES
# undef HAVE_FONTCONFIG

FILE* p_freetype = NULL;


#define UNICODE
#define FONT_DIR_NT L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts"


char *strcasestr (const char *psz_big, const char *psz_little)
{
	char *p_pos = (char *)psz_big;

	if( !*psz_little ) return p_pos;

	while( *p_pos )
	{
		if( toupper( *p_pos ) == toupper( *psz_little ) )
		{
			char * psz_cur1 = p_pos + 1;
			char * psz_cur2 = (char *)psz_little + 1;
			while( *psz_cur1 && *psz_cur2 &&
				toupper( *psz_cur1 ) == toupper( *psz_cur2 ) )
			{
				psz_cur1++;
				psz_cur2++;
			}
			if( !*psz_cur2 ) return p_pos;
		}
		p_pos++;
	}
	return NULL;
}


static char *FromWide (const wchar_t *wide)
{
	size_t len;
	len = WideCharToMultiByte (CP_UTF8, 0, wide, -1, NULL, 0, NULL, NULL);

	char *out = (char *)malloc (len);
	if (out)
		WideCharToMultiByte (CP_UTF8, 0, wide, -1, out, len, NULL, NULL);
	return out;
}


static int GetFileFontByName( const wchar_t *font_name, char **psz_filename)
{
	HKEY hKey;
	wchar_t vbuffer[MAX_PATH];
	wchar_t dbuffer[256];

	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, FONT_DIR_NT, 0, KEY_READ, &hKey) != ERROR_SUCCESS )
	{
		return 1;
	}

	for( int index = 0;; index++ )
	{
		DWORD vbuflen = MAX_PATH - 1;
		DWORD dbuflen = 255;

		if( RegEnumValueW( hKey, index, vbuffer, &vbuflen,
			NULL, NULL, (LPBYTE)dbuffer, &dbuflen) != ERROR_SUCCESS )
		{
			RegCloseKey(hKey);
			return 2;
		}

		char *psz_value = FromWide( vbuffer );
		char* psz_font_name = FromWide(font_name);

		char *s = strchr( psz_value,'(' );
		if( s != NULL && s != psz_value ) s[-1] = '\0';

		/* Manage concatenated font names */
		if( strchr( psz_value, '&') ) {
			if( strcasestr( psz_value, psz_font_name ) != NULL )
			{
				free(psz_value);
				free(psz_font_name);
				break;
			}
		}
		else {
			if( _stricmp( psz_value, psz_font_name ) == 0 )
			{
				free(psz_value);
				free(psz_font_name);
				break;
			}
		}

		free(psz_value);
		free(psz_font_name);
	}

	 *psz_filename = FromWide( dbuffer );
	RegCloseKey(hKey);

	return 0;
}


static int CALLBACK EnumFontCallback(const ENUMLOGFONTEX *lpelfe, const NEWTEXTMETRICEX *metric,
									 DWORD type, LPARAM lParam)
{
	if(p_freetype)
	{
		fwprintf(p_freetype, L"EnumFontCallback #13\n");
	}


	if( (type & RASTER_FONTTYPE) )
	{
		if(p_freetype)
		{
			fwprintf(p_freetype, L"EnumFontCallback #12 raster:%d, device:%d, truetype:%d\n", 
				type & RASTER_FONTTYPE, type & DEVICE_FONTTYPE, type & TRUETYPE_FONTTYPE);
		}
		return 1;

	}

	fwprintf(p_freetype, L"fullname:%s\n", lpelfe->elfFullName);


	int nRet = GetFileFontByName( lpelfe->elfFullName, (char **)lParam );

	if(p_freetype)
	{
		fwprintf(p_freetype, L"ENUMLOGFONTEX #11 fullname:%s, style:%s, script:%s, nRet:%d\n", 
			lpelfe->elfFullName, lpelfe->elfStyle, lpelfe->elfScript, nRet);
	}
	return nRet;
}

static char* Win32_Select( const wchar_t* family, bool b_bold, bool b_italic, int i_size)
{
	p_freetype = _wfopen(L"freetype.log", L"wt,ccs=UNICODE");

	fwprintf(p_freetype, L"Here in Win32_Select, asking for %s\n", family );

	wchar_t wdir[MAX_PATH];
	if( S_OK != SHGetFolderPathW( NULL, CSIDL_FONTS, NULL, SHGFP_TYPE_CURRENT, wdir ) )
	{
		GetWindowsDirectoryW( wdir, MAX_PATH );
		wcscat( wdir, L"\\fonts" );
	}

	char* psz_win_fonts_path = FromWide( wdir );

	/* */
	LOGFONT lf;
	lf.lfCharSet = DEFAULT_CHARSET;
	if( b_italic )
		lf.lfItalic = true;
	if( b_bold )
		lf.lfWeight = FW_BOLD;

	wcsncpy(lf.lfFaceName, (wchar_t*)family, 32);

	/* */
	char *psz_filename = NULL; 
	HDC hDC = GetDC( NULL );

	EnumFontFamiliesEx(hDC, &lf, (FONTENUMPROC)&EnumFontCallback, (LPARAM)&psz_filename, 0);
	ReleaseDC(NULL, hDC);	

	if( psz_filename == NULL )
	{
		free(psz_win_fonts_path);
		fclose(p_freetype);
		return NULL;
	}

	fwprintf( p_freetype, L"%S\\%S\n", psz_win_fonts_path, psz_filename );
	
	if(psz_filename)
	{
		free(psz_filename);
	}
	free(psz_win_fonts_path);
	fclose(p_freetype);
	return NULL;
}








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
		//Win32_Select(L"ËÎÌו", false, false, 16);

		test();
	}

	return nRetCode;
}


