// UnitTab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool96.h"
#include "UnitTab.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "GraphicDev_Tool.h"

// CUnitTab ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTab, CDialog)

CUnitTab::CUnitTab(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTAB, pParent)
{

}

CUnitTab::~CUnitTab()
{
	Release();
}

void CUnitTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UNIT_MAPCOMBO, m_MapCombo);
	DDX_Control(pDX, IDC_UNIT_UNITCOMBO, m_UnitCombo);
	DDX_Control(pDX, IDC_UNIT_MAPLIST, m_MapList);
	DDX_Control(pDX, IDC_UNIT_UNITLIST, m_UnitList);
	DDX_Control(pDX, IDC_DELETE_BTN, m_DelBtn);
	DDX_Control(pDX, IDC_SAVE_BTN, m_SaveBtn);
	DDX_Control(pDX, IDC_UNIT_MAP_SLIDER, m_MapSlider);
	DDX_Control(pDX, IDC_UNIT_SCALE_TEXT, m_ScaleTextCtrl);
	DDX_Control(pDX, IDC_CUR_UNIT_LIST, m_CurUnitList);
}


BEGIN_MESSAGE_MAP(CUnitTab, CDialog)
	ON_CBN_SELCHANGE(IDC_UNIT_MAPCOMBO, &CUnitTab::On_Select_MapCombo)
	ON_CBN_SELCHANGE(IDC_UNIT_UNITCOMBO, &CUnitTab::On_Select_UnitCombo)
	ON_LBN_SELCHANGE(IDC_UNIT_MAPLIST, &CUnitTab::On_Select_Map)
	ON_LBN_SELCHANGE(IDC_UNIT_UNITLIST, &CUnitTab::On_Select_Unit)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_UNIT_MAP_SLIDER, &CUnitTab::On_Move_MapSlider)
END_MESSAGE_MAP()


// CUnitTab �޽��� ó�����Դϴ�.


void CUnitTab::Map_Initialize(void)
{
	// Map map
	if (nullptr != m_pMapTab)
	{
		m_MapStrMap = *m_pMapTab->Get_MapStrMapPtr();

		if (!m_MapStrMap.empty())
		{
			for (auto iter = m_MapStrMap.begin()
				; iter != m_MapStrMap.end()
				; ++iter)
			{
				m_MapCombo.AddString(iter->first.c_str());
			}
		}
	}
	
	// Unit map
	list<wstring> wstrPlayer;
	wstrPlayer.push_back(L"Player");

	list<wstring> wstrNpc;
	wstrNpc.push_back(L"NPC1");
	wstrNpc.push_back(L"NPC2");
	wstrNpc.push_back(L"NPC3");

	list<wstring> wstrMonster;
	wstrMonster.push_back(L"Jelly");
	wstrMonster.push_back(L"PinkWolf");
	wstrMonster.push_back(L"RedWolf");
	wstrMonster.push_back(L"BlueWolf");
	wstrMonster.push_back(L"BlueWolfWizard");
	wstrMonster.push_back(L"CreedWizard");
	wstrMonster.push_back(L"DrakCow");
	wstrMonster.push_back(L"Boss");

	m_UnitStrMap.emplace(L"Player", wstrPlayer);
	m_UnitStrMap.emplace(L"NPC", wstrNpc);
	m_UnitStrMap.emplace(L"Monster", wstrMonster);

	m_UnitCombo.AddString(L"Player");
	m_UnitCombo.AddString(L"NPC");
	m_UnitCombo.AddString(L"Monster");
}

void CUnitTab::Release(void)
{
	m_MapStrMap.clear();
	m_UnitStrMap.clear();
	for_each(m_vecUnit.begin(), m_vecUnit.end(), DeleteObj());
	m_vecUnit.clear();
}

void CUnitTab::Add_Unit(CPoint & point)
{
	// Unit ��ü ����
	UNIT_INFO* pUnit = new UNIT_INFO;

	CString cstrMap;
	CString cstrObjKey;
	m_MapList.GetText(m_MapList.GetCurSel(), cstrMap);
	m_UnitList.GetText(m_UnitList.GetCurSel(), cstrObjKey);
	
	D3DXVECTOR3 vPos = D3DXVECTOR3((float)point.x, (float)point.y, 0.f);
	int iIndex = m_pMap->Get_TileIndex(vPos);
	m_pMap->Tile_Change(vPos, TD_POS, L"");

	STATUS_INFO tInfo = {};
	// TO DO : �ؽ�Ʈ ��Ʈ�� ���ͼ� tInfo ä���
	
	//pUnit->Set_Data(cstrMap.operator LPCWSTR(), cstrObjKey.operator LPCWSTR(), iIndex, tInfo);
	pUnit->m_iTileIndex = iIndex;
	pUnit->m_tStatusData = tInfo;
	pUnit->m_wstrMap = cstrMap;
	pUnit->m_wstrObjKey = cstrObjKey;
	
	m_vecUnit.push_back(pUnit);


	// ����Ʈ ��Ʈ�ѿ� �߰��ϱ�
	TCHAR szText[MIN_STR] = L""; //���ڿ��� ������ ���� ����
	static int nIndex = m_vecUnit.size()-1;  //����� �ε����� ������ ��������

	UpdateData(TRUE);

	LVITEM lItem;  //�� ����ü�� �̿��ؼ� ��Ͽ� ���� ������ �ۼ�

	lItem.mask = LVIF_TEXT;  //�� ����ü�� pszTextȮ��
	lItem.iItem = nIndex;  //����� ��ȣ�� ����

	lItem.iSubItem = LC_MAP;  //���� �׸��� ��ȣ�� ����-�ֻ����̹Ƿ� 0�� �ݴϴ�.
	lstrcpy(szText, cstrMap.operator LPCWSTR());
	lItem.pszText = szText;  //��ϳ��� ����
	m_CurUnitList.InsertItem(&lItem);   //�ֻ����߰�

	lItem.iSubItem = LC_OBJECT;  //�����׸��ȣ 1�� ����.
	lstrcpy(szText, cstrObjKey.operator LPCWSTR());
	lItem.pszText = szText;
	m_CurUnitList.SetItem(&lItem);  //��������߰��� SetItem�Լ��� ���

	lItem.iSubItem = LC_INDEX;  //�����׸��ȣ 1�� ����.
	wsprintf(szText, L"%d", iIndex);
	lItem.pszText = szText;
	m_CurUnitList.SetItem(&lItem);  //��������߰��� SetItem�Լ��� ���


	nIndex++;   //���ο� ��� �߰��ϱ� ���� 1 ����
	UpdateData(FALSE);


}

void CUnitTab::Delete_Unit(void)
{
}

void CUnitTab::Load(void)
{
	CString cstrPath = L"../Data/Unit/Unit.dat";

	HANDLE	hFile = CreateFile(
		cstrPath,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD		dwByte;

	while (true)
	{
		UNIT_INFO*		pUnit = new UNIT_INFO;

		ReadFile(hFile, pUnit, sizeof(UNIT_INFO), &dwByte, NULL);

		if (0 == dwByte)
		{
			Safe_Delete(pUnit);
			break;
		}

		m_vecUnit.push_back(pUnit);
	}

	CloseHandle(hFile);

	// ����Ʈ ��Ʈ�ѿ� �߰��ϱ�
	UpdateData(TRUE);
	TCHAR szText[MIN_STR] = L""; //���ڿ��� ������ ���� ����
	for (int i = 0; i < m_vecUnit.size(); ++i)
	{
		LVITEM lItem;  //�� ����ü�� �̿��ؼ� ��Ͽ� ���� ������ �ۼ�

		lItem.mask = LVIF_TEXT;  //�� ����ü�� pszTextȮ��
		lItem.iItem = i;  //����� ��ȣ�� ����

		lItem.iSubItem = LC_MAP;  //���� �׸��� ��ȣ�� ����-�ֻ����̹Ƿ� 0�� �ݴϴ�.
		lstrcpy(szText, m_vecUnit[i]->m_wstrMap.c_str());
		lItem.pszText = szText;  //��ϳ��� ����
		m_CurUnitList.InsertItem(&lItem);   //�ֻ����߰�

		lItem.iSubItem = LC_OBJECT;  //�����׸��ȣ 1�� ����.
		lstrcpy(szText, m_vecUnit[i]->m_wstrObjKey.c_str());
		lItem.pszText = szText;
		m_CurUnitList.SetItem(&lItem);  //��������߰��� SetItem�Լ��� ���

		lItem.iSubItem = LC_INDEX;  //�����׸��ȣ 1�� ����.
		wsprintf(szText, L"%d", m_vecUnit[i]->m_iTileIndex);
		lItem.pszText = szText;
		m_CurUnitList.SetItem(&lItem);  //��������߰��� SetItem�Լ��� ���
	}

	UpdateData(FALSE);
}

void CUnitTab::Save(void)
{
	CString cstrPath = L"../Data/Unit/Unit.dat";

	HANDLE	hFile = CreateFile(
		cstrPath,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD		dwByte;

	for (auto pUnit : m_vecUnit)
	{
		WriteFile(hFile, pUnit, sizeof(UNIT_INFO), &dwByte, NULL);
	}

	CloseHandle(hFile);
}

BOOL CUnitTab::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// �� ��
	m_pMapTab = ((CMainFrame*)AfxGetMainWnd())->m_pFormView->m_pMapDialog;

	// �����̴�
	m_MapSlider.SetRange(0, 200);
	m_MapSlider.SetPos(100);
	m_MapSlider.SetTicFreq(1);
	m_MapSlider.SetPageSize(1);

	// �߰��� ���� �����͸� ǥ���ϴ� ����Ʈ
	TCHAR* szText[LC_END] = { L"��", L"����", L"Ÿ�� �ε���" };
	int nWid[LC_END] = { 70, 150, 100 };
	LV_COLUMN lCol;

	lCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;  //����ü�� ����� Ȯ���� �÷��׸� ����
	lCol.fmt = LVCFMT_LEFT;  //Į���� ������ �������ķ� ����(_CENTER, _LEFT, _RIGHT�� ����)

	for (int i = 0; i<LC_END; i++) {
		lCol.pszText = szText[i];  //Į���� ������ ����
		lCol.iSubItem = i;  //����������� �ε����� ����
		lCol.cx = nWid[i];  //Į���� ���̸� ����
		m_CurUnitList.InsertColumn(i, &lCol);  //LVCOLUMN����ü�� ������� ���� ���� ����Ʈ ��Ʈ�ѿ� Į���� ����
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CUnitTab::On_Select_MapCombo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int		iSelect = m_MapCombo.GetCurSel();
	CString cstrCategory = L"";
	m_MapCombo.GetLBText(iSelect, cstrCategory);

	auto iter = m_MapStrMap.find(cstrCategory.operator LPCWSTR());

	if (m_MapStrMap.end() == iter) return;
	
	m_MapList.ResetContent();

	for (auto wstrIter = iter->second.begin()
		; wstrIter != iter->second.end()
		; wstrIter++)
	{
		m_MapList.AddString(wstrIter->c_str());
	}
}


void CUnitTab::On_Select_UnitCombo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int		iSelect = m_UnitCombo.GetCurSel();
	CString cstrCategory = L"";
	m_UnitCombo.GetLBText(iSelect, cstrCategory);

	auto iter = m_UnitStrMap.find(cstrCategory.operator LPCWSTR());
	
	if (m_UnitStrMap.end() == iter) return;

	m_UnitList.ResetContent();

	for (auto wstrIter = iter->second.begin()
		; wstrIter != iter->second.end()
		; wstrIter++)
	{
		m_UnitList.AddString(wstrIter->c_str());
	}
}


void CUnitTab::On_Select_Map()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iSelect = m_MapList.GetCurSel();

	if (0 > iSelect)
		return;

	CString cstrStage;
	m_MapList.GetText(iSelect, cstrStage);
	m_pMap->Set_Stage(cstrStage);

	m_pMap->Release(); // Ÿ�� ���ֱ�
	m_pMap->Load_File();
	m_pMap->Render();
}


void CUnitTab::On_Select_Unit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CUnitTab::On_Move_MapSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iPos = m_MapSlider.GetPos();
	float fScale = 0.008f * iPos + 0.2f;

	wchar_t szScale[MIN_STR] = L"x";
	swprintf_s(szScale, L"x%.2f", fScale);
	m_ScaleTextCtrl.SetWindowTextW(szScale);

	UpdateData(FALSE);

	m_pMap->Set_Scale(fScale);
	m_pMap->Render();

	*pResult = 0;
}
