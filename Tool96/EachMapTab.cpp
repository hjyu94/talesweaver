// EachMapTab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool96.h"
#include "EachMapTab.h"
#include "afxdialogex.h"


// CEachMapTab ��ȭ �����Դϴ�.

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


// CEachMapTab �޽��� ó�����Դϴ�.


void CEachMapTab::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (NULL == m_EachMapTool.GetSafeHwnd())
	{
		m_EachMapTool.Create(IDD_EACHMAPTOOL);
	}

	m_EachMapTool.ShowWindow(SW_SHOW);

}
