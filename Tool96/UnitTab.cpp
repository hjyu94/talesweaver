// UnitTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool96.h"
#include "UnitTab.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "GraphicDev_Tool.h"

// CUnitTab 대화 상자입니다.

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


// CUnitTab 메시지 처리기입니다.


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
	// Unit 객체 생성
	UNIT_INFO* pUnit = new UNIT_INFO;

	CString cstrMap;
	CString cstrObjKey;
	m_MapList.GetText(m_MapList.GetCurSel(), cstrMap);
	m_UnitList.GetText(m_UnitList.GetCurSel(), cstrObjKey);
	
	D3DXVECTOR3 vPos = D3DXVECTOR3((float)point.x, (float)point.y, 0.f);
	int iIndex = m_pMap->Get_TileIndex(vPos);
	m_pMap->Tile_Change(vPos, TD_POS, L"");

	STATUS_INFO tInfo = {};
	// TO DO : 텍스트 컨트롤 얻어와서 tInfo 채우기
	
	//pUnit->Set_Data(cstrMap.operator LPCWSTR(), cstrObjKey.operator LPCWSTR(), iIndex, tInfo);
	pUnit->m_iTileIndex = iIndex;
	pUnit->m_tStatusData = tInfo;
	pUnit->m_wstrMap = cstrMap;
	pUnit->m_wstrObjKey = cstrObjKey;
	
	m_vecUnit.push_back(pUnit);


	// 리스트 컨트롤에 추가하기
	TCHAR szText[MIN_STR] = L""; //문자열을 저장할 변수 선언
	static int nIndex = m_vecUnit.size()-1;  //목록의 인덱스를 저장할 변수선언

	UpdateData(TRUE);

	LVITEM lItem;  //이 구조체를 이용해서 목록에 대한 정보를 작성

	lItem.mask = LVIF_TEXT;  //이 구조체의 pszText확장
	lItem.iItem = nIndex;  //목록의 번호를 지정

	lItem.iSubItem = LC_MAP;  //세부 항목의 번호를 지정-최상목록이므로 0을 줍니다.
	lstrcpy(szText, cstrMap.operator LPCWSTR());
	lItem.pszText = szText;  //목록내용 지정
	m_CurUnitList.InsertItem(&lItem);   //최상목록추가

	lItem.iSubItem = LC_OBJECT;  //세부항목번호 1로 지정.
	lstrcpy(szText, cstrObjKey.operator LPCWSTR());
	lItem.pszText = szText;
	m_CurUnitList.SetItem(&lItem);  //하위목록추가는 SetItem함수를 사용

	lItem.iSubItem = LC_INDEX;  //세부항목번호 1로 지정.
	wsprintf(szText, L"%d", iIndex);
	lItem.pszText = szText;
	m_CurUnitList.SetItem(&lItem);  //하위목록추가는 SetItem함수를 사용


	nIndex++;   //새로운 목록 추가하기 위해 1 증가
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

	// 리스트 컨트롤에 추가하기
	UpdateData(TRUE);
	TCHAR szText[MIN_STR] = L""; //문자열을 저장할 변수 선언
	for (int i = 0; i < m_vecUnit.size(); ++i)
	{
		LVITEM lItem;  //이 구조체를 이용해서 목록에 대한 정보를 작성

		lItem.mask = LVIF_TEXT;  //이 구조체의 pszText확장
		lItem.iItem = i;  //목록의 번호를 지정

		lItem.iSubItem = LC_MAP;  //세부 항목의 번호를 지정-최상목록이므로 0을 줍니다.
		lstrcpy(szText, m_vecUnit[i]->m_wstrMap.c_str());
		lItem.pszText = szText;  //목록내용 지정
		m_CurUnitList.InsertItem(&lItem);   //최상목록추가

		lItem.iSubItem = LC_OBJECT;  //세부항목번호 1로 지정.
		lstrcpy(szText, m_vecUnit[i]->m_wstrObjKey.c_str());
		lItem.pszText = szText;
		m_CurUnitList.SetItem(&lItem);  //하위목록추가는 SetItem함수를 사용

		lItem.iSubItem = LC_INDEX;  //세부항목번호 1로 지정.
		wsprintf(szText, L"%d", m_vecUnit[i]->m_iTileIndex);
		lItem.pszText = szText;
		m_CurUnitList.SetItem(&lItem);  //하위목록추가는 SetItem함수를 사용
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 맵 탭
	m_pMapTab = ((CMainFrame*)AfxGetMainWnd())->m_pFormView->m_pMapDialog;

	// 슬라이더
	m_MapSlider.SetRange(0, 200);
	m_MapSlider.SetPos(100);
	m_MapSlider.SetTicFreq(1);
	m_MapSlider.SetPageSize(1);

	// 추가된 유닛 데이터를 표시하는 리스트
	TCHAR* szText[LC_END] = { L"맵", L"종류", L"타일 인덱스" };
	int nWid[LC_END] = { 70, 150, 100 };
	LV_COLUMN lCol;

	lCol.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;  //구조체의 기능을 확장할 플래그를 지정
	lCol.fmt = LVCFMT_LEFT;  //칼럼의 정렬을 왼쪽정렬로 지정(_CENTER, _LEFT, _RIGHT중 선택)

	for (int i = 0; i<LC_END; i++) {
		lCol.pszText = szText[i];  //칼럼의 제목을 지정
		lCol.iSubItem = i;  //서브아이템의 인덱스를 지정
		lCol.cx = nWid[i];  //칼럼의 넓이를 지정
		m_CurUnitList.InsertColumn(i, &lCol);  //LVCOLUMN구조체로 만들어진 값을 토대로 리스트 컨트롤에 칼럼을 삽입
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUnitTab::On_Select_MapCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSelect = m_MapList.GetCurSel();

	if (0 > iSelect)
		return;

	CString cstrStage;
	m_MapList.GetText(iSelect, cstrStage);
	m_pMap->Set_Stage(cstrStage);

	m_pMap->Release(); // 타일 없애기
	m_pMap->Load_File();
	m_pMap->Render();
}


void CUnitTab::On_Select_Unit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CUnitTab::On_Move_MapSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
