#pragma once

#include "Include.h"

// CPathFind ��ȭ �����Դϴ�.

class CPathFind : public CDialog
{
	DECLARE_DYNAMIC(CPathFind)

public:
	CPathFind(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathFind();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHFIND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CListBox m_ListBox;

public:
	list<SINGLE_IMGPATH*>			m_PathList;

public:
	afx_msg void OnListBox();
	afx_msg void OnSaveFile();
	afx_msg void OnLoadFile();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
