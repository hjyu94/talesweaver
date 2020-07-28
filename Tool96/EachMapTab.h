#pragma once


// CEachMapTab 대화 상자입니다.

#include "EachMapTool.h"

class CEachMapTab : public CDialogEx
{
	DECLARE_DYNAMIC(CEachMapTab)

public:
	CEachMapTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEachMapTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

private:
	CEachMapTool m_EachMapTool;
};
