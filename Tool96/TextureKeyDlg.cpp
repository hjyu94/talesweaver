// TextureKeyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool96.h"
#include "TextureKeyDlg.h"
#include "afxdialogex.h"

#include "SpriteTab.h"

// CTextureKeyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTextureKeyDlg, CDialog)

CTextureKeyDlg::CTextureKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TEXTUREKEYDLG, pParent)
{

}

CTextureKeyDlg::~CTextureKeyDlg()
{
}

void CTextureKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KEY_OBJ_EDIT, m_ObjKeyEdit);
	DDX_Control(pDX, IDC_KEY_STATE_EDIT, m_StateKeyEdit);
	DDX_Control(pDX, IDC_KEY_PATH_COMBO, m_PathCombo);
	DDX_Control(pDX, IDC_KEY_B_OBJ_EDIT, m_BStateKeyEdit);
	DDX_Control(pDX, IDC_KEY_B_STATE_EDIT, m_BObjKeyEdit);
}


BEGIN_MESSAGE_MAP(CTextureKeyDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTextureKeyDlg::On_Click_Ok)
	ON_CBN_SELCHANGE(IDC_KEY_PATH_COMBO, &CTextureKeyDlg::On_Select_Path)
	ON_EN_KILLFOCUS(IDC_KEY_OBJ_EDIT, &CTextureKeyDlg::On_Killfocus_ObjEdit)
	ON_EN_KILLFOCUS(IDC_KEY_STATE_EDIT, &CTextureKeyDlg::On_Killfocus_StateEdit)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTextureKeyDlg 메시지 처리기입니다.


void CTextureKeyDlg::Init(void)
{
	m_vecPathPtr = m_pSpriteTab->Get_PathVectorPtr();

	if (nullptr == m_vecPathPtr)
		return;
	
	m_PathCombo.ResetContent();

	for (int i = 0; i < m_vecPathPtr->size(); ++i)
	{
		m_PathCombo.AddString((*m_vecPathPtr)[i]->tTool_Info.wstrPath.c_str());
	}
	m_iCurComboIdx = 0;
	m_PathCombo.SetCurSel(m_iCurComboIdx);
	IMGPATH* pPath = (*m_vecPathPtr)[m_iCurComboIdx];
	m_BObjKeyEdit.SetWindowTextW(pPath->wstrObjKey.c_str());
	m_BStateKeyEdit.SetWindowTextW(pPath->wstrStateKey.c_str());
}

BOOL CTextureKeyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	//return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTextureKeyDlg::On_Click_Ok()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pSpriteTab->Add_To_PathList();
	CDialog::OnOK();
}


void CTextureKeyDlg::On_Select_Path()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iCurComboIdx = m_PathCombo.GetCurSel();

	IMGPATH* pPath = (*m_vecPathPtr)[m_iCurComboIdx];
	m_BObjKeyEdit.SetWindowTextW(pPath->wstrObjKey.c_str());
	m_BStateKeyEdit.SetWindowTextW(pPath->wstrStateKey.c_str());
}


void CTextureKeyDlg::OnDestroy()
{
	m_vecPathPtr->clear();
	CDialog::OnDestroy();
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CTextureKeyDlg::On_Killfocus_ObjEdit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	IMGPATH* pPath = (*m_vecPathPtr)[m_iCurComboIdx];

	CString cstrObjKey = L"";
	m_ObjKeyEdit.GetWindowTextW(cstrObjKey);
	pPath->wstrObjKey = cstrObjKey.operator LPCWSTR();
}

void CTextureKeyDlg::On_Killfocus_StateEdit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	IMGPATH* pPath = (*m_vecPathPtr)[m_iCurComboIdx];

	CString cstrStateKey = L"";
	m_StateKeyEdit.GetWindowTextW(cstrStateKey);
	pPath->wstrStateKey = cstrStateKey.operator LPCWSTR();
}
