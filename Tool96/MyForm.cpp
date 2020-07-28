// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool96.h"
#include "MyForm.h"
#include "MainFrm.h"
#include "GraphicDev_Tool.h"

// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
	, m_pMapDialog(nullptr)
	, m_pSpriteTab(nullptr)
	, m_pUnitTab(nullptr)
	, m_pPathFind(nullptr)
{

}

CMyForm::~CMyForm()
{
	Safe_Delete(m_pMapDialog);
	Safe_Delete(m_pSpriteTab);
	Safe_Delete(m_pUnitTab);
	Safe_Delete(m_pPathFind);
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_CtrlTab);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyForm::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기입니다.


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_CtrlTab.DeleteAllItems();
	m_CtrlTab.InsertItem(TAB_MAP, L"Map");
	m_CtrlTab.InsertItem(TAB_UNIT, L"Unit");
	m_CtrlTab.InsertItem(TAB_SPRITE, L"Sprite");
	m_CtrlTab.InsertItem(TAB_PATH_FIND, L"SingleTex");

	m_CtrlTab.SetCurSel(TAB_MAP);

	CRect rect;
	this->GetWindowRect(&rect);

	m_pMapDialog = new CMapTab;
	m_pMapDialog->Create(IDD_MAP_DLG, &m_CtrlTab);
	m_pMapDialog->MoveWindow(5, 25, rect.Width(), rect.Height());
	m_pMapDialog->ShowWindow(SW_SHOW);

	m_pUnitTab = new CUnitTab;
	m_pUnitTab->Create(IDD_UNITTAB, &m_CtrlTab);
	m_pUnitTab->MoveWindow(5, 25, rect.Width(), rect.Height());
	m_pUnitTab->ShowWindow(SW_HIDE);

	m_pSpriteTab = new CSpriteTab;
	m_pSpriteTab->Create(IDD_SPRITE_DLG, &m_CtrlTab);
	m_pSpriteTab->MoveWindow(5, 25, rect.Width(), rect.Height());
	m_pSpriteTab->ShowWindow(SW_HIDE);

	m_pPathFind = new CPathFind;
	m_pPathFind->Create(IDD_PATHFIND, &m_CtrlTab);
	m_pPathFind->MoveWindow(5, 25, rect.Width(), rect.Height());
	m_pPathFind->ShowWindow(SW_HIDE);

	m_CtrlTab.MoveWindow(0, 0, rect.Width(), rect.Height());

	UpdateData(FALSE);

	m_pMainView = ((CMainFrame*)AfxGetMainWnd())->m_pMainView;

	UpdateData(FALSE);
}


void CMyForm::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int select = m_CtrlTab.GetCurSel();
	RECT crt = {};

	switch (select)
	{
	case ::TAB_MAP:
		m_pSpriteTab->On_Pause();

		// 화면 지우기
		CGraphicDev::Get_Instance()->Render_Begin();
		CGraphicDev::Get_Instance()->Render_End();

		m_pMapDialog->ShowWindow(SW_SHOW);
		m_pUnitTab->ShowWindow(SW_HIDE);
		m_pSpriteTab->ShowWindow(SW_HIDE);
		m_pPathFind->ShowWindow(SW_HIDE);
		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Set_eTab(TAB_MAP);

		break;

	case::TAB_UNIT:
		m_pSpriteTab->On_Pause();

		// 화면 지우기
		CGraphicDev::Get_Instance()->Render_Begin();
		CGraphicDev::Get_Instance()->Render_End();

		m_pMapDialog->ShowWindow(SW_HIDE);
		m_pUnitTab->ShowWindow(SW_SHOW);
		m_pSpriteTab->ShowWindow(SW_HIDE);
		m_pPathFind->ShowWindow(SW_HIDE);
		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Set_eTab(TAB_UNIT);

		break;


	case ::TAB_SPRITE:
		// 화면 지우기
		CGraphicDev::Get_Instance()->Render_Begin();
		CGraphicDev::Get_Instance()->Render_End();

		m_pMapDialog->ShowWindow(SW_HIDE);
		m_pUnitTab->ShowWindow(SW_HIDE);
		m_pSpriteTab->ShowWindow(SW_SHOW);
		m_pPathFind->ShowWindow(SW_HIDE);
		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Set_eTab(TAB_SPRITE);

		break;

	case TAB_PATH_FIND:
		m_pSpriteTab->On_Pause();
		m_pMapDialog->ShowWindow(SW_HIDE);
		m_pUnitTab->ShowWindow(SW_HIDE);
		m_pSpriteTab->ShowWindow(SW_HIDE);
		m_pPathFind->ShowWindow(SW_SHOW);
		((CMainFrame*)AfxGetMainWnd())->m_pMainView->Set_eTab(TAB_SPRITE);

		break;
	}

	*pResult = 0;
}
