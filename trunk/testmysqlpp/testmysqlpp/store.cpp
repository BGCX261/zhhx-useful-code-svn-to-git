
#include "stdafx.h"
#include <mysql++.h>
#include <iostream>
#include <iconv.h>
#include <errno.h>
#include <tchar.h>

using namespace std;
using namespace mysqlpp;


#ifdef WIN32
static inline char *FromWide (const wchar_t *wide, char* pOut, int nOutLen)
{
	size_t len = WideCharToMultiByte (CP_UTF8, 0, wide, -1, NULL, 0, NULL, NULL);
	if (len == 0)
		return NULL;

	if (pOut)
		WideCharToMultiByte (CP_UTF8, 0, wide, -1, pOut, nOutLen, NULL, NULL);
	return pOut;
}


static inline wchar_t *ToWide (const char *utf8, wchar_t * pOut, int nOutLen)
{
	size_t len = MultiByteToWideChar (CP_UTF8, 0, utf8, -1, NULL, 0);
	if (len == 0)
		return NULL;

	if (pOut)
		MultiByteToWideChar (CP_UTF8, 0, utf8, -1, pOut, nOutLen);
	return pOut;
}


int MultiByteToWideChar1(LPCSTR lpcSrcStr, LPWSTR lpwDstStr, int nDstSize)
{
	int size = MultiByteToWideChar(CP_ACP, 0, lpcSrcStr, -1, NULL, 0);

	if( nDstSize > size )
	{
		return MultiByteToWideChar(CP_ACP, 0, lpcSrcStr, -1, lpwDstStr, size);
	}
	return 0;
}

int WideCharToMultiByte1(LPWSTR lpwSrcStr, LPSTR lpcDstStr, int nSize)
{
	int size = WideCharToMultiByte(CP_OEMCP, NULL, lpwSrcStr, -1, NULL, 0, NULL, FALSE);

	if( nSize > size)
	{
		return WideCharToMultiByte(CP_OEMCP, NULL, lpwSrcStr, -1, lpcDstStr, nSize, NULL, FALSE);
	}
	return 0;
}
#endif


int testmysqlpp(  )
{
//	iconv_t igu = iconv_open("utf8", "gb18030");

//	iconv_t iug = iconv_open("gb2312", "utf8");

	Connection conn(false);

	//Option* pOption = new SetCharsetNameOption("gb2312");

	//bool bRet = conn.set_option(pOption);

	if( conn.connect("example", "192.168.1.106", "root", "123456"))
	{
		Query query = conn.query("select username, password, role, refcount from t_user");
		if( StoreQueryResult res = query.store() )
		{
			StoreQueryResult::const_iterator it;
			for(it = res.begin() ; it != res.end(); it++ )
			{
				Row row = *it;
				char achUsername[512] = {0};
				size_t iUsernameLen = 512;
				char achInput[512] = {0};
				size_t nFieldLen = row[0].length();
				strncpy(achInput, (char*)row[0].c_str(), nFieldLen);
				char* pInput = achInput;
				char* pOutput = achUsername;
				TCHAR user[512] = {0};
				ToWide(achInput, user, 511);
				WideCharToMultiByte1(user, achUsername, 511);
				cout<< '\t' << achUsername << '\t' << row[1] << '\t' << row[2] << '\t' << row[3] << endl;    
			}
		}
		query.reset();

		TCHAR* pchUser = _T("ÖÐ¹ú");
		size_t nUserLen = _tcslen(pchUser);

		char achUser[512] = {0};
		size_t nUserBufLen = 512;
		char* pUser = achUser;

//		size_t nRet = iconv(igu, (const char**)&pchUser, &nUserLen, &pUser, &nUserBufLen);

		FromWide(pchUser, achUser, 511);

		string strInsert = "insert into t_user (username, password,role,refcount) values('";
		strInsert += achUser;
		strInsert += "','9dd4e12',1,0)";
		String str(strInsert);
		query << str;
		bool ret = query.exec();
//		iconv_close(igu);
//		iconv_close(iug);


		Query blobQuery = conn.query("select * from t_img");
		if( StoreQueryResult result = blobQuery.store() )
		{
			StoreQueryResult::const_iterator it;
			for(it = result.begin() ; it != result.end(); it++ )
			{
				Row row = *it;
				sql_blob img = row[1];
				int len = img.length();
				char* pImg = new char[len];
				memcpy(pImg, img.data(), len);    
			}
		}

		unsigned char achImg[16] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26};
		string strBin;
		strBin.assign((char*)achImg, 16);
		Query binquery = conn.query();
		binquery << "insert into t_img(id, image) values(2,\"" << mysqlpp::escape << strBin << "\")";
			
		bool bBinRet = binquery.exec();

		return 0;
	}
	return 0;
}


