#pragma once


// CEachMapTab ��ȭ �����Դϴ�.

#include "EachMapTool.h"

class CEachMapTab : public CDialogEx
{
	DECLARE_DYNAMIC(CEachMapTab)

public:
	CEachMapTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEachMapTab();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

private:
	CEachMapTool m_EachMapTool;
};
