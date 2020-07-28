// SpriteTab.cpp : implementation file
//

#include "stdafx.h"
#include "Tool96.h"
#include "SpriteTab.h"
#include "afxdialogex.h"
#include "FileInfo.h"

#include "TextureMgr_Tool.h"
#include "GraphicDev_Tool.h"

// CSpriteTab dialog

IMPLEMENT_DYNAMIC(CSpriteTab, CDialogEx)

CSpriteTab::CSpriteTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SPRITE_DLG, pParent)
{

}

CSpriteTab::~CSpriteTab()
{
	for_each(m_PathVector.begin(), m_PathVector.end(), DeleteObj());
	m_PathVector.clear();

	for (auto iter : m_MapPngImage)
	{
		iter.second->Destroy();
		Safe_Delete(iter.second);
	}
	m_MapPngImage.clear();
}

void CSpriteTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HJ_SPRITE_TREE, m_TreeCtrl);
	DDX_Control(pDX, IDC_HJ_SPRITE_NUM_LIST, m_SpriteListCtrl);
	DDX_Control(pDX, IDC_HJ_SPRITE_PIC, m_PicCtrl);
	DDX_Control(pDX, IDC_OBJ_KEY_EDIT, m_ObjKeyCtrl);
	DDX_Control(pDX, IDC_STATE_KEY_EDIT, m_StateKeyCtrl);
	DDX_Control(pDX, IDC_PATH_STATIC, m_PathStrCtrl);
	DDX_Control(pDX, IDC_PATH_LIST, m_PathListCtrl);
	DDX_Control(pDX, IDC_PLAY_BTN, m_PlayBtn);
	DDX_Control(pDX, IDC_PAUSE_BTN, m_PauseBtn);
	DDX_Control(pDX, IDC_R_EDIT, m_RColorCtrl);
	DDX_Control(pDX, IDC_G_EDIT, m_GColorCtrl);
	DDX_Control(pDX, IDC_B_EDIT, m_BColorCtrl);
	DDX_Control(pDX, IDC_COLOR_KEY_PIC, m_ColKeyPic);
	DDX_Control(pDX, IDC_REVERSE_CHECK, m_ReverseCheck);
	DDX_Control(pDX, IDC_SPRITE_SPEED_SLIDER, m_SliderCtrl);
	DDX_Control(pDX, IDC_SPRITE_SPEED_TEXT, m_SpriteSpeedText);
	DDX_Control(pDX, IDC_FX_EDIT, m_PosXCtrl);
	DDX_Control(pDX, IDC_FY_EDIT, m_PosYCtrl);
	DDX_Control(pDX, IDC_POS_CHECKBOX, m_PosCheck);
	DDX_Control(pDX, IDC_EDIT_BTN, m_EditBtn);
	DDX_Control(pDX, IDC_RESET_BTN, m_ResetBtn);
	DDX_Control(pDX, IDC_DEL_BTN, m_DelBtn);
	DDX_Control(pDX, IDC_LOAD_BTN, m_LoadBtn);
	DDX_Control(pDX, IDC_COLOR_CHECKBOX, m_ColorCheck);
	DDX_Control(pDX, IDC_KEY_CHECK, m_KeyCheckBox);
}


BEGIN_MESSAGE_MAP(CSpriteTab, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_HJ_SPRITE_TREE, &CSpriteTab::On_Select_Tree)
	ON_BN_CLICKED(IDC_ADD_PATH_BTN, &CSpriteTab::On_Add)
	ON_LBN_SELCHANGE(IDC_HJ_SPRITE_NUM_LIST, &CSpriteTab::On_Select_Sprite)
	ON_BN_CLICKED(IDC_PLAY_BTN, &CSpriteTab::On_Play)
	ON_STN_CLICKED(IDC_HJ_SPRITE_PIC, &CSpriteTab::On_Click_Picture)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PAUSE_BTN, &CSpriteTab::On_Pause)
	ON_BN_CLICKED(IDC_REVERSE_CHECK, &CSpriteTab::On_Reverse_Check)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SPRITE_SPEED_SLIDER, &CSpriteTab::On_Move_Sprite_Speed_Slider)
	ON_BN_CLICKED(IDC_POS_CHECKBOX, &CSpriteTab::On_Middle_Point_Check)
	ON_BN_CLICKED(IDC_EDIT_BTN, &CSpriteTab::On_Edit)
	ON_BN_CLICKED(IDC_RESET_BTN, &CSpriteTab::On_Reset)
	ON_BN_CLICKED(IDC_DEL_BTN, &CSpriteTab::On_Delete)
	ON_BN_CLICKED(IDC_LOAD_BTN, &CSpriteTab::On_Save)
	ON_LBN_SELCHANGE(IDC_PATH_LIST, &CSpriteTab::On_Select_Path)
	ON_BN_CLICKED(IDC_COPY_BTN, &CSpriteTab::On_Copy)
END_MESSAGE_MAP()


// CSpriteTab message handlers


void CSpriteTab::Make_Tree(const wstring & wstrPath, HTREEITEM & hParent)
{
	CFileFind Find;

	Find.FindFile(wstrPath.c_str());
	int iContinue = 1;

	while (iContinue)
	{
		iContinue = Find.FindNextFile();

		//1) ".", ".."
		if (Find.IsDots())
			continue;

		//2) directory
		else if (Find.IsDirectory())
		{
			HTREEITEM hItem;
			hItem = m_TreeCtrl.InsertItem(Find.GetFileTitle(), 0, 0, hParent, TVI_LAST);
			Make_Tree(wstring(Find.GetFilePath() + L"\\*.*"), hItem);
		}

		//3) file
		else
		{
			////3-1) system file
			//if (Find.IsSystem())
			//	continue;
		}

	}
}

void CSpriteTab::Update_Sprite_List_And_Cnt(const wstring & wstrPath)
{
	m_SpriteListCtrl.ResetContent();

	CFileFind Find;

	wchar_t szBuf[MAX_PATH];
	lstrcpy(szBuf, wstrPath.c_str());
	PathRemoveFileSpec(szBuf);
	lstrcat(szBuf, L"\\*.png");
	Find.FindFile(szBuf);

	/*wstring wstrTemp = wstrPath + L"\\*.png";
	Find.FindFile(wstrTemp.c_str());
	*/

	int iContinue = 1;

	m_iSpriteCnt = 0;

	while (iContinue)
	{
		m_iSpriteCnt++;
		iContinue = Find.FindNextFile();
		m_SpriteListCtrl.AddString(Find.GetFileTitle());
	}
}


void CSpriteTab::Load_Prev_Img(const CString& cstrPath, const CString& cstrObjKey, const CString& cstrStateKey)
{
	auto iter = m_MapPngImage.find(cstrObjKey + L"|" + cstrStateKey);

	m_PicCtrl.SetBitmap(NULL);

	CImage* pPngImage = nullptr;

	// 미리보기 이미지가 없으면 추가하기
	if (iter == m_MapPngImage.end())
	{
		pPngImage = new CImage;

		wchar_t szPrevPath[MAX_PATH] = L"";
		lstrcpy(szPrevPath, cstrPath.operator LPCWSTR());
		PathRemoveFileSpec(szPrevPath);
		lstrcat(szPrevPath, L"\\*.png");
		wcout << szPrevPath << endl;

		CFileFind Find;
		Find.FindFile(szPrevPath);
		Find.FindNextFile();
		
		pPngImage->Load(Find.GetFilePath());
		m_MapPngImage.emplace(cstrObjKey + L"|" + cstrStateKey, pPngImage);
	}
	else
	{
		pPngImage = iter->second;
	}

	CRect Crere;
	m_PicCtrl.GetClientRect(&Crere);
	pPngImage->Draw(m_PicCtrl.GetDC()->m_hDC, Crere.TopLeft().x, Crere.TopLeft().y, 100, 100);

	//m_PicCtrl.SetWindowPos(NULL, 0, 0, 100, 100, SWP_NOMOVE);
}

bool CSpriteTab::Is_Reverse_Btn_Checked(void)
{
	return 1 == m_ReverseCheck.GetCheck();
}

bool CSpriteTab::Is_Middle_Pos_Btn_Checked(void)
{
	return 1 == m_PosCheck.GetCheck();
}

bool CSpriteTab::Is_ColorKey_Btn_Checked(void)
{
	return 1 == ((CButton*)GetDlgItem(IDC_COLOR_CHECKBOX))->GetCheck();
}

void CSpriteTab::Horizontal_Scroll(void)
{
	CString			strName;
	CSize			csize;
	int				iDCX = 0;

	CDC*			pDC = m_PathListCtrl.GetDC();

	for (int i = 0; i < m_PathListCtrl.GetCount(); ++i)
	{
		m_PathListCtrl.GetText(i, strName);
		csize = pDC->GetTextExtent(strName);	// 인자값으로 들어오는 문자열의 크기를 픽셀 크기로 변환해주는 함수

		if (csize.cx > iDCX)
			iDCX = csize.cx;
	}
	m_PathListCtrl.ReleaseDC(pDC);

	if (m_PathListCtrl.GetHorizontalExtent() < iDCX)
	{
		m_PathListCtrl.SetHorizontalExtent(iDCX);
	}
}


void CSpriteTab::Set_Middle_Pos(POINT& point)
{
	point.x -= 400;
	point.y -= 300;

	m_pSprite->Set_Mid_Point(point);

	wchar_t szBuf[MIN_STR] = L"";
	wsprintf(szBuf, L"%d", point.x);
	m_PosXCtrl.SetWindowTextW(szBuf);
	wsprintf(szBuf, L"%d", point.y);
	m_PosYCtrl.SetWindowTextW(szBuf);

	CGraphicDev::Get_Instance()->Render_Begin();
	m_pSprite->Render();
	CGraphicDev::Get_Instance()->Render_End();
}

BOOL CSpriteTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	
	//# 텍스쳐 트리 구성
	wstring wstrSpritePath = L"../Texture/*.*";
	
	HTREEITEM root;
	root = m_TreeCtrl.InsertItem(L"Texture", 0, 0, TVI_ROOT, TVI_LAST);
	
	Make_Tree(wstrSpritePath, root);

	//# 속도 슬라이더 초기화
	m_SliderCtrl.SetRange(0, 200);
	m_SliderCtrl.SetPos(100);
	m_SliderCtrl.SetTicFreq(1);

	//# 저장된 데이터 불러오기
	Load_Path_Data();
	Horizontal_Scroll();

	//#
	m_TextureKeyDlg.Set_SpriteTab(this);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CSpriteTab::On_Select_Tree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	
	HTREEITEM hItemCur;
	hItemCur = pNMTreeView->itemNew.hItem;
	HTREEITEM hChild = m_TreeCtrl.GetChildItem(hItemCur);

	/***********************************************************************/
	if (NULL == hChild) // 제일 마지막 항목이었다면 스프라이트 불러오기
	{
		CString cstrTemp = L"";

		/***********************************************************************/
		//1) StateKey : Basket

		cstrTemp = m_TreeCtrl.GetItemText(hItemCur);
		m_cstrStateKey = cstrTemp;
		m_StateKeyCtrl.SetWindowTextW(m_cstrStateKey);


		/***********************************************************************/
		//2) ObjKey : Basket

		m_cstrObjKey = m_TreeCtrl.GetItemText(m_TreeCtrl.GetParentItem(hItemCur));
		if (m_cstrObjKey == L"Texture")
		{
			m_cstrObjKey = cstrTemp;
			m_ObjKeyCtrl.SetWindowTextW(m_cstrObjKey);
		}
		else
		{
			m_ObjKeyCtrl.SetWindowTextW(m_cstrObjKey);
		}


		/***********************************************************************/
		//3) Path : ..\\Texture\\Basket\\Basket%d

		HTREEITEM hRoot = m_TreeCtrl.GetRootItem();

		while (hItemCur != hRoot)
		{
			hItemCur = m_TreeCtrl.GetParentItem(hItemCur);
			cstrTemp = m_TreeCtrl.GetItemText(hItemCur) + L"\\" + cstrTemp;
		}
		cstrTemp = L"..\\" + cstrTemp; // [cstrTemp] ..\\Texture\\Basket
	
		
		CFileFind Find;
		Find.FindFile(cstrTemp+L"\\*.png"); // ..\\Texture\\Basket\\*.png 맞는지 확인해야함!
		Find.FindNextFile();

		wstring wstrFile = Find.GetFileTitle(); // Basket0
		wstrFile = wstrFile.substr(0, wstrFile.size() - 1); // Basket
		wstrFile += L"%d.png"; // Basket%d.png

		m_cstrDirPath = cstrTemp + L"\\" + wstrFile.c_str();
		::SetDlgItemText(this->m_hWnd, IDC_PATH_STATIC, m_cstrDirPath);


		/***********************************************************************/
		//##) ObjKey, StateKey, Path 출력

		// UpdateData(TRUE); // 컨트롤->변수 (변수 값 수정)
		UpdateData(FALSE); // 변수->컨트롤 (출력)


		/***********************************************************************/
		//##2) 비트맵 출력하기
		//m_cstrPrevPath = Find.GetFilePath();
		Load_Prev_Img(m_cstrDirPath.operator LPCWSTR(), m_cstrObjKey.operator LPCWSTR(), m_cstrStateKey.operator LPCWSTR());
		

		/***********************************************************************/
		//##3) 파일 개수 구하고, 리스트에 추가하기
		
		m_SpriteListCtrl.ResetContent();
		//Update_Sprite_List_And_Cnt(cstrTemp.operator LPCWSTR());
		// [cstrTemp] ..\\Texture\\Basket
		Update_Sprite_List_And_Cnt(m_cstrDirPath.operator LPCWSTR());

		/***********************************************************************/
		//##4) 텍스쳐 집어넣기
		
		wchar_t wstrSpriteCnt[MIN_STR] = L"";

		m_PathStrCtrl.GetWindowTextW(m_cstrDirPath);
		m_ObjKeyCtrl.GetWindowTextW(m_cstrObjKey);
		m_StateKeyCtrl.GetWindowTextW(m_cstrStateKey);
		wsprintf(wstrSpriteCnt, L"%d", m_iSpriteCnt);

		// 텍스쳐 삽입
		CTextureMgr::Get_Instance()->InsertTexture(
			m_cstrDirPath.operator LPCWSTR()
			, m_cstrObjKey.operator LPCWSTR()
			, TEX_MULTI
			, NULL
			, m_cstrStateKey.operator LPCWSTR()
			, m_iSpriteCnt
		);
	}
	/***********************************************************************/

	else
	{
		::SetDlgItemText(this->m_hWnd, IDC_TEST_STATIC, L"_");
		m_SpriteListCtrl.ResetContent();
	}

	*pResult = 0;
}


void CSpriteTab::On_Add()
{
	// TODO: Add your control notification handler code here
	
	// PathList 초기화
	for_each(m_PathVector.begin(), m_PathVector.end(), DeleteObj());
	m_PathVector.clear();

	// 추가할 Path 원소들을 리스트에 추가하기
	HTREEITEM hSelectedItem = m_TreeCtrl.GetSelectedItem(); // Texture
	Make_Path_List(hSelectedItem);

	if (TRUE == m_KeyCheckBox.GetCheck())
	{
		//m_TextureKeyDlg.Set_Point(D3DXVECTOR3((float)point.x, (float)point.y, 0.f));
		if (NULL == m_TextureKeyDlg.GetSafeHwnd())
		{
			m_TextureKeyDlg.Create(IDD_TEXTUREKEYDLG);
		}
		m_TextureKeyDlg.Init();
		m_TextureKeyDlg.ShowWindow(SW_SHOW);
	}
	else
	{
		Add_To_PathList();
	}
}


void CSpriteTab::On_Select_Sprite()
{
	// TODO: Add your control notification handler code here
	int iCnt = m_SpriteListCtrl.GetCurSel();

	if (0 > iCnt || m_iSpriteCnt <= iCnt)
		return;

	CTextureMgr::Get_Instance()->InsertTexture(
		m_cstrDirPath.operator LPCWSTR()
		, m_cstrObjKey.operator LPCWSTR()
		, TEX_MULTI
		, NULL
		, m_cstrStateKey.operator LPCWSTR()
		, m_iSpriteCnt
	);

	const TEXINFO*		pTexture = CTextureMgr::Get_Instance()->GetTexture(
		m_cstrObjKey.operator LPCWSTR()
		, m_cstrStateKey.operator LPCWSTR()
		, iCnt
	);

	m_pSprite->Set_pTexture(pTexture);

	CGraphicDev::Get_Instance()->Render_Begin();
	m_pSprite->Render();
	CGraphicDev::Get_Instance()->Render_End();
}


void CSpriteTab::On_Click_Picture()
{
	// TODO: Add your control notification handler code here
	
	//# 마우스 커서 위치 받아오기
	POINT mouse;
	GetCursorPos(&mouse);
	GetDlgItem(IDC_HJ_SPRITE_PIC)->ScreenToClient(&mouse);
	cout << mouse.x << ", " << mouse.y << endl;


	//# 비트맵 가져오기
	if (m_cstrDirPath != L"")
	{
		COLORREF rgb = GetPixel(m_PicCtrl.GetDC()->m_hDC, mouse.x, mouse.y);
		m_RValue = GetRValue(rgb);
		m_GValue = GetGValue(rgb);
		m_BValue = GetBValue(rgb);

		wchar_t szBuf[MIN_STR] = L"";

		_itow_s(m_RValue, szBuf, 10);
		m_RColorCtrl.SetWindowTextW(szBuf);

		_itow_s(m_GValue, szBuf, 10);
		m_GColorCtrl.SetWindowTextW(szBuf);

		_itow_s(m_BValue, szBuf, 10);
		m_BColorCtrl.SetWindowTextW(szBuf);

		//# picture contrl 에 색 출력하기
		CDC* pDC = m_ColKeyPic.GetWindowDC();

		CRect Rect = {};
		m_ColKeyPic.GetClientRect(&Rect);

		CBrush myBrush(RGB(m_RValue, m_GValue, m_BValue));
		CBrush* oldBrush;

		oldBrush = pDC->SelectObject(&myBrush);
		pDC->Rectangle(0, 0, Rect.right, Rect.bottom);

		pDC->SelectObject(oldBrush);
		m_ColKeyPic.ReleaseDC(pDC);
	}

	UpdateData(FALSE); // 변수->컨트롤 (출력)
}


void CSpriteTab::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	int iCnt = m_SpriteListCtrl.GetCurSel();

	if (iCnt >= m_iSpriteCnt - 1)
		m_SpriteListCtrl.SetCurSel(0);
	else
		m_SpriteListCtrl.SetCurSel(iCnt + 1);

	On_Select_Sprite();
}

void CSpriteTab::On_Play()
{
	// TODO: Add your control notification handler code here

	// 타이머
	SetTimer(0, m_iSpriteSpeed, NULL);
	// 100 ms 마다 OnTimer 가 실행되도록 한다.
	m_bIsTimerOn = true;

}

void CSpriteTab::On_Pause()
{
	// TODO: Add your control notification handler code here
	KillTimer(0);
	m_bIsTimerOn = false;
}


void CSpriteTab::On_Reverse_Check()
{
	// TODO: Add your control notification handler code here
	CGraphicDev::Get_Instance()->Render_Begin();
	m_pSprite->Render();
	CGraphicDev::Get_Instance()->Render_End();
}

void CSpriteTab::On_Middle_Point_Check()
{
	// TODO: Add your control notification handler code here
	CGraphicDev::Get_Instance()->Render_Begin();
	m_pSprite->Render();
	CGraphicDev::Get_Instance()->Render_End();
}

void CSpriteTab::On_Move_Sprite_Speed_Slider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	int iPos = m_SliderCtrl.GetPos();
	
	m_iSpriteSpeed = iPos;

	wchar_t szBuf[MIN_STR] = L"";
	wsprintf(szBuf, L"%d", iPos);

	SetDlgItemText(IDC_SPRITE_SPEED_TEXT, szBuf);
	
	if (m_bIsTimerOn)
	{
		On_Pause();
		On_Play();
	}

	UpdateData(FALSE);

	*pResult = 0;
}




void CSpriteTab::On_Edit()
{
	// TODO: Add your control notification handler code here
	On_Delete();

	CString cstrCombine = L"";
	
	CString cstrTemp = L"";
	wchar_t szBuf[MIN_STR] = L"";

	wchar_t wstrSpriteCnt[MIN_STR] = L"";
	wsprintf(wstrSpriteCnt, L"%d", m_iSpriteCnt);

	m_ObjKeyCtrl.GetWindowTextW(m_cstrObjKey);
	m_StateKeyCtrl.GetWindowTextW(m_cstrStateKey);

	// PathFind
	cstrCombine = m_cstrObjKey + L"|" + m_cstrStateKey + L"|";
	cstrCombine += wstrSpriteCnt;
	cstrCombine += L"|" + m_cstrDirPath;

	//0) 프레임 속도
	m_SpriteSpeedText.GetWindowTextW(cstrTemp);
	cstrCombine += L"|" + cstrTemp;
	
	//1) 뒤집기
	if (Is_Reverse_Btn_Checked())
		cstrCombine += L"|TRUE";
	else
		cstrCombine += L"|FALSE";

	//2) 색
	if (Is_ColorKey_Btn_Checked())
	{
		wsprintf(szBuf, L"|%d", m_RValue);
		cstrCombine += szBuf;

		wsprintf(szBuf, L",%d", m_GValue);
		cstrCombine += szBuf;

		wsprintf(szBuf, L",%d", m_BValue);
		cstrCombine += szBuf;
	}
	else
		cstrCombine += L"|X";

	//3) 중점
	if (Is_Middle_Pos_Btn_Checked())
	{
		m_PosXCtrl.GetWindowTextW(cstrTemp);
		cstrCombine += L"|" + cstrTemp;
		m_PosYCtrl.GetWindowTextW(cstrTemp);
		cstrCombine += L"," + cstrTemp;
	}
	else
		cstrCombine += L"|X";


	m_PathListCtrl.AddString(cstrCombine);

	// 가로 스크롤
	Horizontal_Scroll();
}


void CSpriteTab::On_Reset()
{
	// TODO: Add your control notification handler code here
	m_PathListCtrl.ResetContent();
}


void CSpriteTab::On_Delete()
{
	// TODO: Add your control notification handler code here
	int iSel = m_PathListCtrl.GetCurSel();
	
	if (0 > iSel || m_PathListCtrl.GetCount() <= iSel)
		return;

	m_PathListCtrl.DeleteString(iSel);
}


void CSpriteTab::On_Save()
{
	// TODO: Add your control notification handler code here
	wofstream			SaveFile;

	SaveFile.open(L"../Data/ImgPath.txt", ios::out);
	
	wchar_t szBuf[MAX_STR] = L"";

	int iCnt = m_PathListCtrl.GetCount();
	for (int i = 0; i < iCnt; ++i)
	{
		m_PathListCtrl.GetText(i, szBuf);

		if (i == iCnt-1)
			SaveFile << szBuf << flush;
		else
			SaveFile << szBuf << endl;
	}

	SaveFile.close();
	MessageBox(L"../Data/ImgPath.txt 저장 완료.", L"Save Success!", MB_OK);

	UpdateData(FALSE);
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}

void CSpriteTab::Load_Path_Data(void)
{
	UpdateData(TRUE);

	wifstream		LoadFile;

	LoadFile.open(L"../Data/ImgPath.txt", ios::in);

	m_PathListCtrl.ResetContent();

	wchar_t szBuf[MAX_STR] = L"";

	while (!LoadFile.eof())
	{
		LoadFile.getline(szBuf, MAX_STR);
		m_PathListCtrl.AddString(szBuf);

	}
	LoadFile.close();

	UpdateData(FALSE);
}

void CSpriteTab::Make_Path_List(HTREEITEM hSelected)
{
	HTREEITEM hCur = hSelected;

	while (true)
	{
		if (m_TreeCtrl.ItemHasChildren(hCur))
		{
			hCur = m_TreeCtrl.GetChildItem(hCur);
		}
		else
		{
			m_PathVector.push_back(Create_ImgPath(hCur));

			if (hCur == hSelected) // 처음에 누른 항목이 제일 하위 항목일 경우
				return;

			while (true)
			{
				if (m_TreeCtrl.GetNextSiblingItem(hCur) != NULL) // Sibling 이 있다면
				{
					hCur = m_TreeCtrl.GetNextSiblingItem(hCur);
					break;
				}
				else
				{
					hCur = m_TreeCtrl.GetParentItem(hCur); // [hCur] Walk_LD -> Pet_Big
					if (hCur == hSelected)
						return;
				}
			}
		}
	}
}

IMGPATH * CSpriteTab::Create_ImgPath(HTREEITEM hItem)
{
	IMGPATH* pImgPath = new IMGPATH;

	CString cstrTemp = L"";


	/***********************************************************************/
	//1) StateKey : Basket

	cstrTemp = m_TreeCtrl.GetItemText(hItem);
	m_cstrStateKey = cstrTemp;
	m_StateKeyCtrl.SetWindowTextW(m_cstrStateKey);
	pImgPath->wstrStateKey = m_cstrStateKey;

	/***********************************************************************/
	//2) ObjKey : Basket

	m_cstrObjKey = m_TreeCtrl.GetItemText(m_TreeCtrl.GetParentItem(hItem));
	if (m_cstrObjKey == L"Texture")
	{
		m_cstrObjKey = cstrTemp;
		m_ObjKeyCtrl.SetWindowTextW(m_cstrObjKey);
	}
	else
	{
		m_ObjKeyCtrl.SetWindowTextW(m_cstrObjKey);
	}

	pImgPath->wstrObjKey = m_cstrObjKey;

	/***********************************************************************/
	//3) Path : ..\\Texture\\Basket\\Basket%d

	HTREEITEM hRoot = m_TreeCtrl.GetRootItem();

	while (hItem != hRoot)
	{
		hItem = m_TreeCtrl.GetParentItem(hItem);
		cstrTemp = m_TreeCtrl.GetItemText(hItem) + L"\\" + cstrTemp;
	}
	cstrTemp = L"..\\" + cstrTemp; // [cstrTemp] ..\\Texture\\Basket


	CFileFind Find;
	Find.FindFile(cstrTemp + L"\\*.png"); // ..\\Texture\\Basket\\*.png 맞는지 확인해야함!
	Find.FindNextFile();

	wstring wstrFile = Find.GetFileTitle(); // Basket0
	wstrFile = wstrFile.substr(0, wstrFile.size() - 1); // Basket
	wstrFile += L"%d.png"; // Basket%d.png

	m_cstrDirPath = cstrTemp + L"\\" + wstrFile.c_str();
	::SetDlgItemText(this->m_hWnd, IDC_PATH_STATIC, m_cstrDirPath);

	pImgPath->tTool_Info.wstrPath = m_cstrDirPath;

	/***********************************************************************/
	//##) ObjKey, StateKey, Path 출력

	// UpdateData(TRUE); // 컨트롤->변수 (변수 값 수정)
	UpdateData(FALSE); // 변수->컨트롤 (출력)

	/***********************************************************************/
	//##2) 비트맵 출력하기
	//m_cstrPrevPath = Find.GetFilePath();
	Load_Prev_Img(m_cstrDirPath.operator LPCWSTR(), m_cstrObjKey.operator LPCWSTR(), m_cstrStateKey.operator LPCWSTR());

	/***********************************************************************/
	//##3) 파일 개수 구하고, 리스트에 추가하기

	m_SpriteListCtrl.ResetContent();
	Update_Sprite_List_And_Cnt(m_cstrDirPath.operator LPCWSTR()); // [cstrTemp] ..\\Texture\\Basket
	//Update_Sprite_List_And_Cnt(cstrTemp.operator LPCWSTR()); // [cstrTemp] ..\\Texture\\Basket
	pImgPath->tTool_Info.iCount = m_iSpriteCnt;

	/***********************************************************************/
	//##4) 텍스쳐 집어넣기

	wchar_t wstrSpriteCnt[MIN_STR] = L"";

	m_PathStrCtrl.GetWindowTextW(m_cstrDirPath);
	m_ObjKeyCtrl.GetWindowTextW(m_cstrObjKey);
	m_StateKeyCtrl.GetWindowTextW(m_cstrStateKey);
	wsprintf(wstrSpriteCnt, L"%d", m_iSpriteCnt);

	// 텍스쳐 삽입
	CTextureMgr::Get_Instance()->InsertTexture(
		m_cstrDirPath.operator LPCWSTR()
		, m_cstrObjKey.operator LPCWSTR()
		, TEX_MULTI
		, NULL
		, m_cstrStateKey.operator LPCWSTR()
		, m_iSpriteCnt
	);
	
	/***********************************************************************/

	pImgPath->tTool_Info.bReverse = false;
	pImgPath->tTool_Info.fFrameSpeed = 100;
	pImgPath->tTool_Info.ptCenter = POINT{ 0, 0 };
	pImgPath->tTool_Info.tColorKey = NULL;

	return pImgPath;

	//IMGPATH* pImgPath = new IMGPATH;

	//CString cstrTemp = m_TreeCtrl.GetItemText(hItem); // Basket

	////3) Path : ..\\Texture\\Basket\\Basket%d
	//HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
	//HTREEITEM hParent = hItem;

	//while (hParent != hRoot)
	//{
	//	hParent = m_TreeCtrl.GetParentItem(hParent);
	//	cstrTemp = m_TreeCtrl.GetItemText(hParent) + L"\\" + cstrTemp;
	//}
	//cstrTemp = L"..\\" + cstrTemp; // [cstrTemp] ..\\Texture\\Basket


	//CFileFind Find;
	//Find.FindFile(cstrTemp + L"\\*.png"); // ..\\Texture\\Basket\\*.png 맞는지 확인해야함!
	//Find.FindNextFile();

	//wstring wstrFile = Find.GetFileTitle(); // Basket0
	//wstrFile = wstrFile.substr(0, wstrFile.size() - 1); // Basket
	//wstrFile += L"%d.png"; // Basket%d.png

	//m_cstrDirPath = cstrTemp + L"\\" + wstrFile.c_str();
	//::SetDlgItemText(this->m_hWnd, IDC_PATH_STATIC, m_cstrDirPath);

	//pImgPath->tToolInfo.wstrPath = m_cstrDirPath;

	//if (TRUE == m_KeyCheckBox.GetCheck())
	//// 키 임의로 설정해주기
	//{
	//	//m_TextureKeyDlg.Set_Point(D3DXVECTOR3((float)point.x, (float)point.y, 0.f));

	//	if (NULL == m_TextureKeyDlg.GetSafeHwnd())
	//	{
	//		//m_TextureKeyDlg.Set_Map(m_pMap);
	//		m_TextureKeyDlg.Create(IDD_TEXTUREKEYDLG);
	//	}
	//	m_TextureKeyDlg.ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	//1) StateKey : Basket

	//	cstrTemp = m_TreeCtrl.GetItemText(hItem);
	//	m_cstrStateKey = cstrTemp;
	//	m_StateKeyCtrl.SetWindowTextW(m_cstrStateKey);
	//	pImgPath->wstrStateKey = m_cstrStateKey;

	//	/***********************************************************************/
	//	//2) ObjKey : Basket

	//	m_cstrObjKey = m_TreeCtrl.GetItemText(m_TreeCtrl.GetParentItem(hItem));
	//	if (m_cstrObjKey == L"Texture")
	//	{
	//		m_cstrObjKey = cstrTemp;
	//		m_ObjKeyCtrl.SetWindowTextW(m_cstrObjKey);
	//	}
	//	else
	//	{
	//		m_ObjKeyCtrl.SetWindowTextW(m_cstrObjKey);
	//	}

	//	pImgPath->wstrObjKey = m_cstrObjKey;
	//}
	//
	////##) ObjKey, StateKey, Path 출력

	//// UpdateData(TRUE); // 컨트롤->변수 (변수 값 수정)
	//UpdateData(FALSE); // 변수->컨트롤 (출력)

	///***********************************************************************/
	////##2) 비트맵 출력하기
	////m_cstrPrevPath = Find.GetFilePath();
	//Load_Prev_Img(m_cstrDirPath.operator LPCWSTR(), m_cstrObjKey.operator LPCWSTR(), m_cstrStateKey.operator LPCWSTR());

	///***********************************************************************/
	////##3) 파일 개수 구하고, 리스트에 추가하기


 //                                                                                                                                            
 //                                                                                                                                                                         	m_SpriteListCtrl.ResetContent();
	//Update_Sprite_List_And_Cnt(m_cstrDirPath.operator LPCWSTR()); // [cstrTemp] ..\\Texture\\Basket
	////Update_Sprite_List_And_Cnt(cstrTemp.operator LPCWSTR()); // [cstrTemp] ..\\Texture\\Basket
	//pImgPath->tToolInfo.iCount = m_iSpriteCnt;

	///***********************************************************************/
	////##4) 텍스쳐 집어넣기

	//wchar_t wstrSpriteCnt[MIN_STR] = L"";

	//m_PathStrCtrl.GetWindowTextW(m_cstrDirPath);
	//m_ObjKeyCtrl.GetWindowTextW(m_cstrObjKey);
	//m_StateKeyCtrl.GetWindowTextW(m_cstrStateKey);
	//wsprintf(wstrSpriteCnt, L"%d", m_iSpriteCnt);

	//// 텍스쳐 삽입
	//CTextureMgr::Get_Instance()->InsertTexture(
	//	m_cstrDirPath.operator LPCWSTR()
	//	, m_cstrObjKey.operator LPCWSTR()
	//	, TEX_MULTI
	//	, NULL
	//	, m_cstrStateKey.operator LPCWSTR()
	//	, m_iSpriteCnt
	//);
	//
	///***********************************************************************/

	//pImgPath->tToolInfo.bReverse = false;
	//pImgPath->tToolInfo.fFrameSpeed = 100;
	//pImgPath->tToolInfo.ptCenter = POINT{ 0, 0 };
	//pImgPath->tToolInfo.tColorKey = NULL;

	//return pImgPath;
}

void CSpriteTab::Add_To_PathList(void)
{
	// 리스트 박스에 원소들 추가하기
	for (auto& pImgPath : m_PathVector)
	{
		CString cstrCombine = L"";
		CString cstrObjKey = pImgPath->wstrObjKey.c_str();
		CString cstrStateKey = pImgPath->wstrStateKey.c_str();
		wchar_t szBuf[MIN_STR] = L"";
		wsprintf(szBuf, L"%d", pImgPath->tTool_Info.iCount);
		CString cstrPath = pImgPath->tTool_Info.wstrPath.c_str();

		// PathFind
		cstrCombine = cstrObjKey + L"|" + cstrStateKey + L"|";
		cstrCombine += szBuf;
		cstrCombine += L"|" + cstrPath + L"|100|FALSE|X|X";

		m_PathListCtrl.AddString(cstrCombine);

		// 가로 스크롤
		Horizontal_Scroll();
	}
}


void CSpriteTab::On_Select_Path()
{
	// TODO: Add your control notification handler code here

	int iSelect = m_PathListCtrl.GetCurSel();

	if (0 > iSelect || m_PathListCtrl.GetCount() <= iSelect) 
		return;

	wchar_t szBuf[MAX_STR] = L"";
	m_PathListCtrl.GetText(iSelect, szBuf);
	// Basket|Basket|2|..\Texture\Basket\Basket%d.png|100|FALSE|X|X
	// Basket|Basket|2|..\Texture\Basket\Basket%d.png|100|FALSE|255,0,255|10,20
	// ObjKey / StateKey / iSpriteCnt / Path / SpriteSpeed / bReverse / ColorKey / MiddlePos

	CString cstrTemp = L"";
	
	AfxExtractSubString(m_cstrObjKey, szBuf, 0, L'|');
	m_ObjKeyCtrl.SetWindowTextW(m_cstrObjKey);

	AfxExtractSubString(m_cstrStateKey, szBuf, 1, L'|');
	m_StateKeyCtrl.SetWindowTextW(m_cstrStateKey);

	AfxExtractSubString(m_cstrDirPath, szBuf, 3, L'|');
	Load_Prev_Img(m_cstrDirPath, m_cstrObjKey, m_cstrStateKey);
	Update_Sprite_List_And_Cnt(m_cstrDirPath.operator LPCWSTR());
	
	AfxExtractSubString(cstrTemp, szBuf, 4, L'|');
	m_SpriteSpeedText.SetWindowTextW(cstrTemp);
	m_iSpriteSpeed = _ttoi(cstrTemp);
	m_SliderCtrl.SetPos(m_iSpriteSpeed);
	/*
	m_bIsTimerOn = true;
	SetTimer(0, m_iSpriteSpeed, NULL);
	*/

	AfxExtractSubString(cstrTemp, szBuf, 5, L'|');
	if (lstrcmp(cstrTemp, L"TRUE") == 0)
	{
		m_ReverseCheck.SetCheck(TRUE);
	}
	else
	{
		m_ReverseCheck.SetCheck(FALSE);
	}

	AfxExtractSubString(cstrTemp, szBuf, 6, L'|');
	if (lstrcmp(cstrTemp, L"X") == 0)
	{
		m_ColorCheck.SetCheck(FALSE);
		m_RColorCtrl.SetWindowTextW(L"");
		m_GColorCtrl.SetWindowTextW(L"");
		m_BColorCtrl.SetWindowTextW(L"");

		//# picture contrl 에 기본 회색 출력하기
		CDC* pDC = m_ColKeyPic.GetWindowDC();

		CRect Rect = {};
		m_ColKeyPic.GetClientRect(&Rect);

		CBrush myBrush(RGB(240, 240, 240));
		CBrush* oldBrush;

		oldBrush = pDC->SelectObject(&myBrush);
		pDC->Rectangle(0, 0, Rect.right, Rect.bottom);

		pDC->SelectObject(oldBrush);
		m_ColKeyPic.ReleaseDC(pDC);
	}
	else
	{
		m_ColorCheck.SetCheck(TRUE);

		CString cstrColor = L"";
		AfxExtractSubString(cstrColor, cstrTemp, 0, L',');
		m_RColorCtrl.SetWindowTextW(cstrColor);
		m_RValue = _ttoi(cstrColor);

		AfxExtractSubString(cstrColor, cstrTemp, 1, L',');
		m_GColorCtrl.SetWindowTextW(cstrColor);
		m_GValue = _ttoi(cstrColor);

		AfxExtractSubString(cstrColor, cstrTemp, 2, L',');
		m_BColorCtrl.SetWindowTextW(cstrColor);
		m_BValue = _ttoi(cstrColor);

		//# picture contrl 에 색 출력하기
		CDC* pDC = m_ColKeyPic.GetWindowDC();

		CRect Rect = {};
		m_ColKeyPic.GetClientRect(&Rect);

		CBrush myBrush(RGB(m_RValue, m_GValue, m_BValue));
		CBrush* oldBrush;

		oldBrush = pDC->SelectObject(&myBrush);
		pDC->Rectangle(0, 0, Rect.right, Rect.bottom);

		pDC->SelectObject(oldBrush);
		m_ColKeyPic.ReleaseDC(pDC);
	}

	AfxExtractSubString(cstrTemp, szBuf, 7, L'|');
	if (lstrcmp(cstrTemp, L"X") == 0)
	{	
		m_PosCheck.SetCheck(FALSE);
		m_PosXCtrl.SetWindowTextW(L"");
		m_PosYCtrl.SetWindowTextW(L"");
	}
	else
	{
		m_PosCheck.SetCheck(TRUE);

		CString cstrPosVal = L"";

		AfxExtractSubString(cstrPosVal, cstrTemp, 0, L',');
		m_PosXCtrl.SetWindowTextW(cstrPosVal);

		AfxExtractSubString(cstrPosVal, cstrTemp, 1, L',');
		m_PosYCtrl.SetWindowTextW(cstrPosVal);
	}

	UpdateData(FALSE);
}


void CSpriteTab::On_Copy()
{
	// TODO: Add your control notification handler code here
	int iSelect = m_PathListCtrl.GetCurSel();
	CString cstrText = L"";
	m_PathListCtrl.GetText(iSelect, cstrText);
	m_PathListCtrl.AddString(cstrText);
}
