// EachMapTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool96.h"
#include "EachMapTab.h"
#include "afxdialogex.h"


// CEachMapTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEachMapTab, CDialogEx)

CEachMapTab::CEachMapTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CEachMapTab::~CEachMapTab()
{
}

void CEachMapTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEachMapTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CEachMapTab::OnBnClickedButton1)
END_MESSAGE_MAP()


// CEachMapTab 메시지 처리기입니다.


void CEachMapTab::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (NULL == m_EachMapTool.GetSafeHwnd())
	{
		m_EachMapTool.Create(IDD_EACHMAPTOOL);
	}

	m_EachMapTool.ShowWindow(SW_SHOW);

}
