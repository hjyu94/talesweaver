// TextureKeyDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool96.h"
#include "TextureKeyDlg.h"
#include "afxdialogex.h"

#include "SpriteTab.h"

// CTextureKeyDlg ��ȭ �����Դϴ�.

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


// CTextureKeyDlg �޽��� ó�����Դϴ�.


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
	
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	//return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CTextureKeyDlg::On_Click_Ok()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pSpriteTab->Add_To_PathList();
	CDialog::OnOK();
}


void CTextureKeyDlg::On_Select_Path()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iCurComboIdx = m_PathCombo.GetCurSel();

	IMGPATH* pPath = (*m_vecPathPtr)[m_iCurComboIdx];
	m_BObjKeyEdit.SetWindowTextW(pPath->wstrObjKey.c_str());
	m_BStateKeyEdit.SetWindowTextW(pPath->wstrStateKey.c_str());
}


void CTextureKeyDlg::OnDestroy()
{
	m_vecPathPtr->clear();
	CDialog::OnDestroy();
	
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CTextureKeyDlg::On_Killfocus_ObjEdit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	IMGPATH* pPath = (*m_vecPathPtr)[m_iCurComboIdx];

	CString cstrObjKey = L"";
	m_ObjKeyEdit.GetWindowTextW(cstrObjKey);
	pPath->wstrObjKey = cstrObjKey.operator LPCWSTR();
}

void CTextureKeyDlg::On_Killfocus_StateEdit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	IMGPATH* pPath = (*m_vecPathPtr)[m_iCurComboIdx];

	CString cstrStateKey = L"";
	m_StateKeyEdit.GetWindowTextW(cstrStateKey);
	pPath->wstrStateKey = cstrStateKey.operator LPCWSTR();
}
