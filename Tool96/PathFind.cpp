// PathFind.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool96.h"
#include "PathFind.h"
#include "afxdialogex.h"

#include "FileInfo.h"

// CPathFind ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFIND, pParent)
{

}

CPathFind::~CPathFind()
{
	for_each(m_PathList.begin(), m_PathList.end(), DeleteObj());
	m_PathList.clear();
}

void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PATH_FIND_LIST, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_LBN_SELCHANGE(IDC_PATH_FIND_LIST, &CPathFind::OnListBox)
	ON_BN_CLICKED(IDC_PATH_FIND_SAVE_BTN, &CPathFind::OnSaveFile)
	ON_BN_CLICKED(IDC_PATH_FIND_LOAD_BTN, &CPathFind::OnLoadFile)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

void CPathFind::OnListBox()
{
}

void CPathFind::OnSaveFile()
{
	wofstream			SaveFile;

	SaveFile.open(L"../Data/SingleImgPath.txt", ios::out);

	int		iCount = 0;

	for (auto& iter : m_PathList)
	{
		++iCount;

		SaveFile << iter->wstrObjKey.c_str() << L"|";

		if (iCount == m_PathList.size())
		{
			SaveFile << iter->wstrPath.c_str() << flush;
		}
		else
		{
			SaveFile << iter->wstrPath.c_str() << endl;
		}
	}

	SaveFile.close();
	WinExec("notepad.exe ../Data/SingleImgPath.txt", SW_SHOW);
	
}

void CPathFind::OnLoadFile()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	wifstream		LoadFile;

	LoadFile.open(L"../Data/SingleImgPath.txt", ios::in);

	TCHAR		szObjKey[MIN_STR] = L"";
	TCHAR		szImgPath[MAX_PATH] = L"";

	wstring		wstrCombine = L"";

	m_ListBox.ResetContent();

	while (!LoadFile.eof())
	{
		LoadFile.getline(szObjKey, MIN_STR, '|');
		LoadFile.getline(szImgPath, MAX_PATH);
		
		wstrCombine = wstring(szObjKey) + L"|" + wstring(szImgPath);
		m_ListBox.AddString(wstrCombine.c_str());

	}
	// LoadFile.close();

	UpdateData(FALSE);
	WinExec("notepad.exe ../Data/SingleImgPath.txt", SW_SHOW);
}

void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE);

	size_t		iCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	TCHAR	szFullPath[MAX_PATH] = L"";

	for (size_t i = 0; i < iCount; ++i)
	{
		DragQueryFile(hDropInfo, i, (LPWSTR)szFullPath, MAX_PATH);

		CFileInfo		FileInfo;


		FileInfo.DirInfoExtraction(szFullPath, m_PathList);
	}

	for (auto& iter : m_PathList)
	{
		wstring		wstrCombine = L"";
		TCHAR		szBuf[MIN_STR] = L"";

		wstrCombine = iter->wstrObjKey + L"|" + iter->wstrPath;
		m_ListBox.AddString(wstrCombine.c_str());
	}

	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}



// CPathFind �޽��� ó�����Դϴ�.
