
// Tool96View.cpp : CTool96View Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Tool96.h"
#endif

#include "Tool96Doc.h"
#include "Tool96View.h"

#include "GraphicDev_Tool.h"
#include "TextureMgr_Tool.h"

#include "MainFrm.h"
#include "MyForm.h"
#include "Obj.h"
#include "UnitTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd;
int TILEX; // value.h // Ȯ��!!!
int TILEY;

// CTool96View

IMPLEMENT_DYNCREATE(CTool96View, CScrollView)

BEGIN_MESSAGE_MAP(CTool96View, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()

// CTool96View ����/�Ҹ�

CTool96View::CTool96View()
	: m_pGraphicDev(CGraphicDev::Get_Instance())
	, m_pTextureMgr(CTextureMgr::Get_Instance())
	, m_bLButtonClick(false)
	, m_eTab(TAB_MAP)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CTool96View::~CTool96View()
{
}

BOOL CTool96View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CTool96View �׸���

void CTool96View::OnDraw(CDC* /*pDC*/)
{
	CTool96Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pGraphicDev->Render_Begin();

	switch (m_eTab)
	{
	case TAB_MAP:
		m_pMap->Render();
		/*m_pBack->Render();
		for (size_t i = 0; i < m_vecWall.size(); ++i)
		{
			m_vecWall[i]->Render();
		}*/
		break;

	case TAB_SPRITE:
		m_pSprite->Render();
		break;
	}

	m_pGraphicDev->Render_End();
}


// CTool96View �μ�

BOOL CTool96View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CTool96View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CTool96View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CTool96View ����

#ifdef _DEBUG
void CTool96View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTool96View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTool96Doc* CTool96View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTool96Doc)));
	return (CTool96Doc*)m_pDocument;
}
#endif //_DEBUG


// CTool96View �޽��� ó����

void CTool96View::OnInitialUpdate()
{
	g_hWnd = m_hWnd;

	//SetScrollSizes(MM_TEXT, CSize(TILECX * 30, (TILECY / 2) * 20));
	SetScrollSizes(MM_TEXT, CSize(0, 0));
	// 1. �ؽ�Ʈ�� �ȼ� ũ������� ��ũ�� �� ���̸� �����ϰڴٴ� ���ڰ�

	CMainFrame*		pMainFrm = ((CMainFrame*)AfxGetMainWnd());

	RECT		rcWindow;
	pMainFrm->GetWindowRect(&rcWindow);	// ���� â�� ������ ����� ������ �Լ�

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT		rcMainView;
	GetClientRect(&rcMainView);	// ���� �� â�� ũ�⸦ ������ �Լ�

	float		fRowFrm = float(rcWindow.right - rcMainView.right);
	float		fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	pMainFrm->SetWindowPos(NULL,
		200,
		300,
		int(WINCX + fRowFrm),
		int(WINCY + fColFrm),
		SWP_NOZORDER);

	if (FAILED(m_pGraphicDev->Initialize()))
	{
		AfxMessageBox(L"����̽� �ʱ�ȭ ����");
		return;
	}


	((CMainFrame*)AfxGetMainWnd())->m_pFormView->m_pMapDialog->Initialize();
	((CMainFrame*)AfxGetMainWnd())->m_pFormView->m_pUnitTab->Map_Initialize();

	m_pMap = new CHJ_Map;
	m_pMap->Initialize();
	m_pMap->Set_MainView(this);

	m_pSprite = new CSprite;
	m_pSprite->Initialize();
	m_pSprite->SetMainView(this);

	CScrollView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}


void CTool96View::OnDestroy()
{
	CScrollView::OnDestroy();

	Safe_Delete(m_pMap);
	Safe_Delete(m_pSprite);

	m_pTextureMgr->Destroy_Instance();
	m_pGraphicDev->Destroy_Instance();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CTool96View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	switch (m_eTab)
	{
	case TAB_MAP:
		point.x += GetScrollPos(0);
		point.y += GetScrollPos(1);
		m_pMap->Get_pMapTab()->On_LButton_Down_On_View(point);
		m_pMap->Render();
		break;

	case TAB_UNIT:
		m_pMap->Get_pUnitTab()->Add_Unit(point);
		break;


	case TAB_SPRITE:
		if (m_pSprite->Get_pSpriteTab()->Is_Middle_Pos_Btn_Checked())
		{
			m_pSprite->Get_pSpriteTab()->Set_Middle_Pos(point);
		}
		break;
	}
	
	m_bLButtonClick = true;

	CScrollView::OnLButtonDown(nFlags, point);
}

void CTool96View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	m_bLButtonClick = false;

	CScrollView::OnLButtonUp(nFlags, point);
}

void CTool96View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	// Ŭ���ϸ鼭 �巡���ϴ� ��� ó��
	if (m_bLButtonClick)
	{
		if (GetAsyncKeyState(VK_LBUTTON))
			OnLButtonDown(nFlags, point);
	}

	// ������Ʈ �̸����� ����
	switch (m_eTab)
	{
	case TAB_MAP:
		m_pMap->Get_pMapTab()->On_Mouse_Move(point);
		break;

	case TAB_UNIT:
		m_pMap->Render();
		break;

	case TAB_SPRITE:
		break;

	}

	CScrollView::OnMouseMove(nFlags, point);
}