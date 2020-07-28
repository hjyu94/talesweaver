#include "stdafx.h"
#include "FileInfo_Client.h"
#include <fstream>

IMPLEMENT_SINGLETON(CFileInfo_Client);

CFileInfo_Client::CFileInfo_Client()
{
}


CFileInfo_Client::~CFileInfo_Client()
{
}

int CFileInfo_Client::Get_TxtFileLine(wstring * arrFile, int iCount)
{
	// »Æ¿Œ
	//int iLineCnt = 0;
	//
	//for (int i = 0; i < iCount; ++i)
	//{
	//	// todo
	//	wifstream		LoadFile;
	//	TCHAR			szTemp[MAX_STR] = L"";
	//	LoadFile.open(arrFile[i], ios::in);
	//	
	//	while (LoadFile.getline(szTemp, MAX_STR, '\n'))
	//		++iLineCnt;

	//	LoadFile.close();
	//}

	//return iLineCnt;

	return 0;
}
