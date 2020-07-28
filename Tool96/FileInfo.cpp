#include "stdafx.h"
#include "FileInfo.h"


std::wstring CFileInfo::ConvertRelativePath(wstring szFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";
	TCHAR	szDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirPath), szDirPath);

	PathRelativePathTo(
		szRelativePath
		// L"..\\Resources\\TalesWeaver\\Stage\\Field0.png"

		,/* (LPCWSTR pszFrom) */ szDirPath
		// L"C:\\Users\\HyoJeong\\source\\repos\\hjyu94\\mfc-team\\Tool96
		
		, FILE_ATTRIBUTE_DIRECTORY
		
		,/* (LPCWSTR pszTo) */ szFullPath.c_str()
		// L"C:\\Users\\HyoJeong\\source\\repos\\hjyu94\\mfc-team\\Resources\\TalesWeaver\\Stage\\Field0.png"
		
		, FILE_ATTRIBUTE_NORMAL
	);

	return szRelativePath;
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathList)
{
	CFileFind				Find;
	// 그 경로의 하위로 모든 파일을 찾는다.파일이 존재하지 않으면 0를 리턴하고, 파일이 존재할 경우 FindNextFile() 함수를 사용한다.
	Find.FindFile(wstrPath.c_str());

	int		iContinue = 1;

	while (iContinue)
	{
		iContinue = Find.FindNextFile();

		// 파일이 '.'또는 '..'파일이면 0이 아닌 수를리턴한다.
		if (Find.IsDots())
			continue;

		if (Find.IsDirectory())
		{
			// 파일의 경로 이름을 반환하는 함수 + *.*
			DirInfoExtraction(wstring(Find.GetFilePath() + L"\\*.*"), rPathList);
		}
		// 일반적인 파일 상태의 경우
		else
		{
			//  시스템 파일
			if (Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPath[MAX_PATH] = L"";

			lstrcpy(szPath, Find.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szPath);

			// 현재 폴더 경로를 기준으로 몇 장의 이미지가 있는지 계산
			pImgPath->tTool_Info.iCount = DirFileCount(szPath);

			// 확장자를 뺀 파일 이름을 반환
			wstring		wstrTextureName = Find.GetFileTitle().operator LPCWSTR();
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";

			TCHAR		szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, (LPWSTR)Find.GetFilePath().operator LPCWSTR());
			// szBuf = 0x003acf78 L"D:\\nalvan\\96\\Frame96\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			PathRemoveFileSpec(szBuf);
			PathCombine(szBuf, szBuf, (LPWSTR)wstrTextureName.c_str());

			//szBuf = 0x003acf78 L"D:\\nalvan\\96\\Frame96\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"
			pImgPath->tTool_Info.wstrPath = ConvertRelativePath(szBuf);
			// pImgPath->wstrPath = L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"


			PathRemoveFileSpec(szBuf);
			// szBuf = 0x003acf78 L"D:\\nalvan\\96\\Frame96\\Texture\\Stage\\Effect\\BossMultiAttack"
			pImgPath->wstrStateKey = PathFindFileName(szBuf); // 현 위치의 파일 이름을 추출
															  // pImgPath->wstrStateKey = L"BossMultiAttack"

			PathRemoveFileSpec(szBuf);
			// szBuf = 0x003acf78 L"D:\\nalvan\\96\\Frame96\\Texture\\Stage\\Effect"
			pImgPath->wstrObjKey = PathFindFileName(szBuf); // 현 위치의 파일 이름을 추출
															// pImgPath->wstrObjKey = L"Effect"

			rPathList.push_back(pImgPath);

			Find.FindFile(Find.GetFilePath());
			Find.FindNextFile();
		}
	}
}

// 확인) 수정
void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<SINGLE_IMGPATH*>& rPathList)
{
	CFileFind Find;
	// 그 경로의 하위로 모든 파일을 찾는다.파일이 존재하지 않으면 0를 리턴하고, 파일이 존재할 경우 FindNextFile() 함수를 사용한다.
	Find.FindFile(wstrPath.c_str());

	int		iContinue = 1;

	while (iContinue)
	{
		iContinue = Find.FindNextFile();

		// 파일이 '.'또는 '..'파일이면 0이 아닌 수를리턴한다.
		if (Find.IsDots())
			continue;

		if (Find.IsDirectory())
		{
			// 파일의 경로 이름을 반환하는 함수 + *.*
			DirInfoExtraction(wstring(Find.GetFilePath() + L"\\*.*"), rPathList);
		}
		// 일반적인 파일 상태의 경우
		else
		{
			//  시스템 파일
			if (Find.IsSystem())
				continue;

			SINGLE_IMGPATH*		pImgPath = new SINGLE_IMGPATH;

			pImgPath->wstrObjKey = Find.GetFileTitle();
			
			wstring wstrPath = ConvertRelativePath(Find.GetFilePath().operator LPCWSTR());
			pImgPath->wstrPath = wstrPath;

			rPathList.push_back(pImgPath);
		}
	}
}

int CFileInfo::DirFileCount(const wstring & wstrPath)
{
	wstring		wstTemp = wstrPath + L"\\*.png";

	CFileFind		Find;
	Find.FindFile(wstTemp.c_str());

	int		iContinue = 1;
	int		iFileCount = 0;

	while (iContinue)
	{
		iContinue = Find.FindNextFile();

		if (!Find.IsDirectory())
			++iFileCount;
	}

	return iFileCount;
}
CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}
