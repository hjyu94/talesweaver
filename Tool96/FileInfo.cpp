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
	// �� ����� ������ ��� ������ ã�´�.������ �������� ������ 0�� �����ϰ�, ������ ������ ��� FindNextFile() �Լ��� ����Ѵ�.
	Find.FindFile(wstrPath.c_str());

	int		iContinue = 1;

	while (iContinue)
	{
		iContinue = Find.FindNextFile();

		// ������ '.'�Ǵ� '..'�����̸� 0�� �ƴ� ���������Ѵ�.
		if (Find.IsDots())
			continue;

		if (Find.IsDirectory())
		{
			// ������ ��� �̸��� ��ȯ�ϴ� �Լ� + *.*
			DirInfoExtraction(wstring(Find.GetFilePath() + L"\\*.*"), rPathList);
		}
		// �Ϲ����� ���� ������ ���
		else
		{
			//  �ý��� ����
			if (Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;
			TCHAR			szPath[MAX_PATH] = L"";

			lstrcpy(szPath, Find.GetFilePath().operator LPCWSTR());
			PathRemoveFileSpec(szPath);

			// ���� ���� ��θ� �������� �� ���� �̹����� �ִ��� ���
			pImgPath->tTool_Info.iCount = DirFileCount(szPath);

			// Ȯ���ڸ� �� ���� �̸��� ��ȯ
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
			pImgPath->wstrStateKey = PathFindFileName(szBuf); // �� ��ġ�� ���� �̸��� ����
															  // pImgPath->wstrStateKey = L"BossMultiAttack"

			PathRemoveFileSpec(szBuf);
			// szBuf = 0x003acf78 L"D:\\nalvan\\96\\Frame96\\Texture\\Stage\\Effect"
			pImgPath->wstrObjKey = PathFindFileName(szBuf); // �� ��ġ�� ���� �̸��� ����
															// pImgPath->wstrObjKey = L"Effect"

			rPathList.push_back(pImgPath);

			Find.FindFile(Find.GetFilePath());
			Find.FindNextFile();
		}
	}
}

// Ȯ��) ����
void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<SINGLE_IMGPATH*>& rPathList)
{
	CFileFind Find;
	// �� ����� ������ ��� ������ ã�´�.������ �������� ������ 0�� �����ϰ�, ������ ������ ��� FindNextFile() �Լ��� ����Ѵ�.
	Find.FindFile(wstrPath.c_str());

	int		iContinue = 1;

	while (iContinue)
	{
		iContinue = Find.FindNextFile();

		// ������ '.'�Ǵ� '..'�����̸� 0�� �ƴ� ���������Ѵ�.
		if (Find.IsDots())
			continue;

		if (Find.IsDirectory())
		{
			// ������ ��� �̸��� ��ȯ�ϴ� �Լ� + *.*
			DirInfoExtraction(wstring(Find.GetFilePath() + L"\\*.*"), rPathList);
		}
		// �Ϲ����� ���� ������ ���
		else
		{
			//  �ý��� ����
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
