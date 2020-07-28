// TileInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Tool96.h"
#include "TileInfo.h"
#include "afxdialogex.h"

#include "MapTab.h"
#include "HJ_Map.h"

// CTileInfo dialog

IMPLEMENT_DYNAMIC(CTileInfo, CDialog)

CTileInfo::CTileInfo(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TILEINFO, pParent)
{

}

CTileInfo::~CTileInfo()
{
}

void CTileInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TI_CATEGORY, m_CategoryCombo);
	DDX_Control(pDX, IDC_TI_MAP, m_MapListBox);
	DDX_Control(pDX, IDC_MAP_PIC, m_MapPicCtrl);
}


BEGIN_MESSAGE_MAP(CTileInfo, CDialog)
	ON_CBN_SELCHANGE(IDC_TI_CATEGORY, &CTileInfo::On_Select_Category)
	ON_LBN_SELCHANGE(IDC_TI_MAP, &CTileInfo::On_Select_Map)
	ON_BN_CLICKED(IDOK, &CTileInfo::On_Click_Ok)
END_MESSAGE_MAP()


// CTileInfo message handlers


void CTileInfo::On_Select_Category()
{
	// TODO: Add your control notification handler code here
	map<wstring, list<wstring>>* pMapMap = m_pMapTab->Get_MapStrMapPtr();
	
	int		iSelect = m_CategoryCombo.GetCurSel();
	CString cstrCategory = L"";
	
	m_CategoryCombo.GetLBText(iSelect, cstrCategory);
	
	auto iter = pMapMap->find(cstrCategory.operator LPCWSTR());

	m_MapListBox.ResetContent();

	for (auto wstrIter = iter->second.begin()
		; wstrIter != iter->second.end()
		; wstrIter++)
	{
		m_MapListBox.AddString(wstrIter->c_str());
	}
}


BOOL CTileInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	map<wstring, list<wstring>>* pMapMap = m_pMapTab->Get_MapStrMapPtr();
	
	for (auto iter = pMapMap->begin()
		; iter != pMapMap->end()
		; ++iter)
	{
		m_CategoryCombo.AddString(iter->first.c_str());
	}

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CTileInfo::On_Select_Map()
{
	// TODO: Add your control notification handler code here
	map<CString, CImage*>* pMapMap = m_pMapTab->Get_ImageMapPtr();
	
	int		iSelect = m_MapListBox.GetCurSel();
	CString cstrMap = L"";

	m_MapListBox.GetText(iSelect, cstrMap);
	
	auto& iter = pMapMap->find(cstrMap);
	//m_MapPicCtrl.SetBitmap(*iter->second);

	CRect tRect;
	m_MapPicCtrl.GetClientRect(&tRect);
	iter->second->Draw(m_MapPicCtrl.GetDC()->m_hDC
		, tRect.left, tRect.top
		, tRect.right, tRect.bottom);
}


void CTileInfo::On_Click_Ok()
{
	// TODO: Add your control notification handler code here
	int		iSelect = m_MapListBox.GetCurSel();
	CString cstrMap = L"";

	m_MapListBox.GetText(iSelect, cstrMap);

	m_pMap->Tile_Change(
		m_vPoint, TD_GATE, cstrMap.operator LPCWSTR()
	);
	m_pMap->Render();
	CDialog::OnOK();
}
