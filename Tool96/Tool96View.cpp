
// Tool96View.cpp : CTool96View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
int TILEX; // value.h // 확인!!!
int TILEY;

// CTool96View

IMPLEMENT_DYNCREATE(CTool96View, CScrollView)

BEGIN_MESSAGE_MAP(CTool96View, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()

// CTool96View 생성/소멸

CTool96View::CTool96View()
	: m_pGraphicDev(CGraphicDev::Get_Instance())
	, m_pTextureMgr(CTextureMgr::Get_Instance())
	, m_bLButtonClick(false)
	, m_eTab(TAB_MAP)
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CTool96View::~CTool96View()
{
}

BOOL CTool96View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CTool96View 그리기

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


// CTool96View 인쇄

BOOL CTool96View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTool96View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTool96View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CTool96View 진단

#ifdef _DEBUG
void CTool96View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTool96View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTool96Doc* CTool96View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTool96Doc)));
	return (CTool96Doc*)m_pDocument;
}
#endif //_DEBUG


// CTool96View 메시지 처리기

void CTool96View::OnInitialUpdate()
{
	g_hWnd = m_hWnd;

	//SetScrollSizes(MM_TEXT, CSize(TILECX * 30, (TILECY / 2) * 20));
	SetScrollSizes(MM_TEXT, CSize(0, 0));
	// 1. 텍스트의 픽셀 크기단위로 스크롤 바 길이를 결정하겠다는 인자값

	CMainFrame*		pMainFrm = ((CMainFrame*)AfxGetMainWnd());

	RECT		rcWindow;
	pMainFrm->GetWindowRect(&rcWindow);	// 현재 창의 프레임 사이즈를 얻어오는 함수

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

	RECT		rcMainView;
	GetClientRect(&rcMainView);	// 순수 뷰 창의 크기를 얻어오는 함수

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
		AfxMessageBox(L"디바이스 초기화 실패");
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

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CTool96View::OnDestroy()
{
	CScrollView::OnDestroy();

	Safe_Delete(m_pMap);
	Safe_Delete(m_pSprite);

	m_pTextureMgr->Destroy_Instance();
	m_pGraphicDev->Destroy_Instance();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CTool96View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// 클릭하면서 드래그하는 경우 처리
	if (m_bLButtonClick)
	{
		if (GetAsyncKeyState(VK_LBUTTON))
			OnLButtonDown(nFlags, point);
	}

	// 오브젝트 미리보기 설정
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