// MapTab.cpp : implementation file
//

#include "stdafx.h"
#include "Tool96.h"
#include "MapTab.h"
#include "afxdialogex.h"
#include "TextureMgr_Tool.h"

#include "HJ_Map.h"
#include "MainFrm.h"
#include "Tool96View.h"
#include "GraphicDev_Tool.h"

#include "Obj.h"

// CMapTab dialog

IMPLEMENT_DYNAMIC(CMapTab, CDialog)

CMapTab::CMapTab(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAP_DLG, pParent)
	, m_pMap(nullptr)
	, m_iStageCnt(0)
	, m_fMapScale(1.f)
	, m_bySelectedTile(0)
	, m_iRadio(0)
{
	ZeroMemory(&m_tObjData, sizeof(OBJ_DATA));
}

CMapTab::~CMapTab()
{
	//#1 ��Ʈ�� �޸� �ݳ�
	for (auto iter : m_MapPngImage)
	{
		iter.second->Destroy();
		Safe_Delete(iter.second);
	}
	m_MapPngImage.clear();

	m_MapMap.clear();
}

void CMapTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAP_CATEGORY_LIST, m_MapCategoryList);
	DDX_Control(pDX, IDC_MAP_LIST, m_MapList);
	DDX_Control(pDX, IDC_TILE_LIST, m_TileList);
	DDX_Control(pDX, IDC_TILE_PIC, m_TilePicture);
	DDX_Control(pDX, IDC_MAP_SCALE_SLIDER, m_MapScaleSlider);
	DDX_Control(pDX, IDC_MAP_SCALE_TEXT, m_MapScaleText);
	DDX_Control(pDX, IDC_OBJ_LIST, m_ObjList);
	DDX_Control(pDX, IDC_OBJ_SCALE_SLIDER, m_ObjScaleSlider);
	DDX_Control(pDX, IDC_OBJ_PIC, m_ObjPicture);
	DDX_Control(pDX, IDC_OBJ_SCALE_TEXT, m_ObjScaleText);
	DDX_Radio(pDX, IDC_OBJ_RADIO, m_iRadio);
	DDX_Control(pDX, IDC_OBJ_COMBO, m_ObjCombo);
}


BEGIN_MESSAGE_MAP(CMapTab, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_TILE_LIST, &CMapTab::On_Select_Tile_Type)
	ON_LBN_SELCHANGE(IDC_MAP_CATEGORY_LIST, &CMapTab::On_Select_Stage_Category)
	ON_LBN_SELCHANGE(IDC_MAP_LIST, &CMapTab::On_Select_Stage)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_MAP_SCALE_SLIDER, &CMapTab::On_Move_Map_Scale_Slider)
	ON_BN_CLICKED(IDC_OBJ_RESET_BTN, &CMapTab::On_Reset_Obj)
	ON_BN_CLICKED(IDC_TILE_RESET_BTN, &CMapTab::On_Reset_Tile)
	//ON_BN_CLICKED(IDC_HJ_MAP_LOAD_BTN, &CMapTab::On_Load)
	ON_BN_CLICKED(IDC_MAP_SAVE_BTN, &CMapTab::On_Save)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_ALPHA_SLIDER, &CMapTab::On_Move_Alpha_Slider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_OBJ_SCALE_SLIDER, &CMapTab::On_Move_Obj_Scale_Slider)
	ON_LBN_SELCHANGE(IDC_OBJ_LIST, &CMapTab::On_Select_Object)
	ON_CBN_SELCHANGE(IDC_OBJ_COMBO, &CMapTab::On_Select_Object_Category)
END_MESSAGE_MAP()


// CMapTab message handlers



void CMapTab::HorizontalScroll(void)
{
	CString		strName;
	CSize		cSize;

	int			iDCX = 0;
	CDC*		pDC = m_MapCategoryList.GetDC();

	for (int i = 0; i < m_MapCategoryList.GetCount(); ++i)
	{
		m_MapCategoryList.GetText(i, strName);
		cSize = pDC->GetTextExtent(strName);

		if (cSize.cx > iDCX) iDCX = cSize.cx;
	}
	m_MapCategoryList.ReleaseDC(pDC);

	if (m_MapCategoryList.GetHorizontalExtent() < iDCX)
	{
		m_MapCategoryList.SetHorizontalExtent(iDCX);
	}
}

void CMapTab::Initialize(void)
{
	//## Ÿ�� �ؽ���
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Tile/Tile%d.png", L"HJ_TILE", TEX_MULTI, NULL, L"Tile", 5)))
	{
		AfxMessageBox(L"���������� Ÿ�� �ؽ��� ���� ����");
	}

	//## ��

	
	//CString tStagePath = _T("C:\\Users\\HyoJeong\\source\\repos\\hjyu94\\talesweaver\\Texture\\Stage");
	CString tStagePath = _T("..\\Texture\\Stage");
	CString tAll = L"\\*.*";
	CString tPath = tStagePath + tAll;

	//�˻� Ŭ����
	CFileFind finder;

	//CFileFind�� ����, ���͸��� �����ϸ� TRUE �� ��ȯ��
	BOOL bWorking = finder.FindFile(tPath);

	CString fileName;
	CString DirName;

	while (bWorking)
	{
		//���� ���� / ���� �� �����ϸ�ٸ� TRUE ��ȯ
		bWorking = finder.FindNextFile();

		// ���͸� �϶�
		if (finder.IsDirectory())
		{
			DirName = finder.GetFileName();

			if (DirName == _T(".") ||
				DirName == _T("..") ||
				DirName == _T("Thumbs.db")) continue;

			m_MapCategoryList.AddString(DirName);

			// Ȯ��
			m_MapMap.emplace(DirName.operator LPCWSTR(), list<wstring>());

			CString cstrInnerPath
				= tStagePath + L"\\" + DirName + tAll;

			CFileFind	InnerFinder;
			BOOL		bInnerWorking = InnerFinder.FindFile(cstrInnerPath);
			int			iCount = 0;

			while (bInnerWorking)
			{
				bInnerWorking = InnerFinder.FindNextFile();

				//if (InnerFinder.IsArchived())
				//if (!InnerFinder.IsDirectory())
				//{
					//������ �̸�
					CString _fileName = InnerFinder.GetFileName();

					// �������� �������� ����������� ����
					if (_fileName == _T(".") ||
						_fileName == _T("..") ||
						_fileName == _T("Thumbs.db")) continue;

					fileName = InnerFinder.GetFileTitle();
					wcout << (const wchar_t*)_fileName << endl;

					// Ȯ��
					auto iter = m_MapMap.find(DirName.operator LPCWSTR());
					iter->second.push_back(fileName.operator LPCWSTR());

					//# ��Ʈ�� ����
					auto& iter2 = m_MapPngImage.find(fileName);

					if (m_MapPngImage.end() == iter2)
					{
						CImage*		pPngImage = new CImage;
						pPngImage->Load(InnerFinder.GetFilePath());
						m_MapPngImage.emplace(fileName, pPngImage);
					}

					iCount++;
				//}
			}

			cout << iCount << endl;

			// �ؽ��� ����ֱ�
			wstring wstrTexPath = tStagePath + L"/" + DirName + L"/" + DirName + "%d.png";
			wstring wstrStateKey = DirName;
			wstring wstrObjKey = L"HJ_" + DirName.MakeUpper();

			if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(wstrTexPath, wstrObjKey, TEX_MULTI, NULL, wstrStateKey, iCount)))
			{
				AfxMessageBox(L"���������� �������� �ؽ��� ���� ����");
			}
		}
	}


	//## Ÿ��
	wstring		wstrRelativePath = L"../Texture/Tile/Tile%d.png";
	TCHAR		szPath[MAX_PATH] = L"";

	for (int i = 0; i < 5; ++i)
	{
		wsprintf(szPath, wstrRelativePath.c_str(), i);

		CString	strImgName;

		// ��θ� �߶󳻰� ������ �̸��� ������� �ϴ� �۾�
		strImgName = PathFindFileName(szPath);

		// Ȯ���ڸ� �߶󳻴� �Լ�
		PathRemoveExtension((LPWSTR)strImgName.operator LPCWSTR()); // ���� ���� ������

																	// Ÿ�� �̹��� �߰��ϱ�
																	// ���� �̸����� �׸��� ���� ���� �̹��� ���� �߰� �۾�
		auto& iter = m_MapPngImage.find(strImgName);

		// �̹����� ���� ��� �߰��ϱ�
		if (m_MapPngImage.end() == iter)
		{
			CImage*		pPngImage = new CImage;
			pPngImage->Load(szPath);
			m_MapPngImage.emplace(strImgName, pPngImage);
			m_TileList.AddString(strImgName);
		}
	}
}

// CTool96View ���� ���콺 ���� ��ư Ŭ�� ��
void CMapTab::On_LButton_Down_On_View(CPoint& point)
{
	UpdateData(TRUE); // ���� ����

	switch (m_iRadio)
	{
	case R_OBJ:
	{
		bool bNew = true;

		// ���� OBJ ��ġ �̵� ��
		vector<CObj*>* pObjVec = m_pMap->Get_ObjVecPtr();
		for (auto iter = pObjVec->begin(); iter != pObjVec->end(); )
		{
			D3DXVECTOR3 vCenter = (*iter)->Get_Center();
			if (abs(point.x - vCenter.x) < 20.f && abs(point.y - vCenter.y) < 20.f)
			{
				iter = pObjVec->erase(iter);
				bNew = false;
			}
			else
			{
				++iter;
			}
		}

		// ���Ӱ� OBJ ����
		if (bNew)
		{
			CString cstrTemp = L"";
			m_tObjData.vPos = D3DXVECTOR3{ (float)point.x, (float)point.y, 0.f };
			m_pMap->Create_Object(point, m_tObjData);
		}
	}
	break;

	case R_TILE:
	{
		TILE_DRAW	eDrawID = (TILE_DRAW)Get_Tile_Type();
		if (eDrawID == TD_GATE)
		{
			m_TileInfo.Set_Point(D3DXVECTOR3((float)point.x, (float)point.y, 0.f));

			if (NULL == m_TileInfo.GetSafeHwnd())
			{
				m_TileInfo.Set_Map(m_pMap);
				m_TileInfo.Create(IDD_TILEINFO);
			}
			m_TileInfo.ShowWindow(SW_SHOW);
		}
		else
		{
			m_pMap->Tile_Change(
				D3DXVECTOR3(float(point.x)/* * fScale*/, float(point.y)/* * fScale*/, 0.f)
				, (TILE_DRAW)Get_Tile_Type(), L""
			);
		}
	}
	break;
	}
}

void CMapTab::On_Mouse_Move(CPoint & point)
{
	if (m_iRadio == R_OBJ /*&& m_ObjList.GetCurSel()*/)
	{
		m_pMap->Get_pPrevObj()->Set_Pos(point);
		m_pMap->Render();
	}
}

//void CMapTab::On_Load(CString cstrStage)
//{
//	// Ȯ��
//	// ���� �ҷ�����
//	/*****************************************************************************/
//	//##1) Ÿ��
//	{
//		CString cstrPath = L"../Data/Tile/" + cstrStage + L".dat";
//		HANDLE	hFile = CreateFile(
//			cstrPath,
//			GENERIC_READ,
//			0,
//			NULL,
//			OPEN_EXISTING,
//			FILE_ATTRIBUTE_NORMAL,
//			NULL);
//
//		vector<TILE*>& vecTile = m_pMap->Get_vecTile();
//
//		DWORD		dwByte;
//		wchar_t		strStageName[MIN_STR];
//		int			iTileX, iTileY;
//
//		ReadFile(hFile, &iTileX, sizeof(int), &dwByte, NULL);
//		ReadFile(hFile, &iTileY, sizeof(int), &dwByte, NULL);
//
//		m_pMap->Set_StageInfo(iTileX, iTileY);
//
//		while (true)
//		{
//			TILE*		pTile = new TILE;
//
//			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
//
//			if (0 == dwByte)
//			{
//				Safe_Delete(pTile);
//				break;
//			}
//
//			vecTile.push_back(pTile);
//		}
//
//		CloseHandle(hFile);
//	}
//
//	/*****************************************************************************/
//	//##2 ������Ʈ
//
//	{
//		CString cstrPath = L"../Data/Obj/" + cstrStage + L".dat";
//		HANDLE		hFile = CreateFile(
//			cstrPath, GENERIC_READ, 0, NULL
//			, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//		DWORD		dwByte = 0;
//		vector<CObj*>* Obj_Vec_Ptr = m_pMap->Get_ObjVecPtr();
//
//		while (true)
//		{
//			CObj*		pObj = new CObj;
//			ReadFile(hFile, pObj, sizeof(CObj), &dwByte, NULL);
//
//			if (dwByte == 0)
//			{
//				Safe_Delete(pObj);
//				break;
//			}
//			
//			Obj_Vec_Ptr->push_back(pObj);
//
//			// Ȯ��
//			wstring wstrStateKey = pObj->Get_Data().wstrStateKey;
//			Insert_Obj_Texture(wstrStateKey, nullptr);
//		}
//
//		CloseHandle(hFile);
//	}
//}

void CMapTab::Set_Map_Size_Text(UINT width, UINT height)
{
	TCHAR szBuf[MID_STR];
	wsprintf(szBuf, L"%d x %d", width, height);

	SetDlgItemText(IDC_STAGE_SIZE_TEXT, szBuf);
}

BOOL CMapTab::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	// ������ �����̴� �ʱ�ȭ
	m_MapScaleSlider.SetRange(0, 200);
	m_MapScaleSlider.SetPos(100);
	m_MapScaleSlider.SetTicFreq(1);
	m_MapScaleSlider.SetPageSize(1);

	//m_AlphaSlider.SetRange(0, 255);
	//m_AlphaSlider.SetPos(255);
	//m_AlphaSlider.SetTicFreq(1);
	//m_AlphaSlider.SetPageSize(10);

	m_ObjScaleSlider.SetRange(1, 30); // 0.1 ~ 3.0
	m_ObjScaleSlider.SetPos(10);
	m_ObjScaleSlider.SetTicFreq(1);
	m_ObjScaleSlider.SetPageSize(1);

	// ������Ʈ �޺��ڽ� ����Ʈ �߰�
	m_ObjCombo.AddString(L"Etc");
	m_ObjCombo.AddString(L"Fence");
	m_ObjCombo.AddString(L"House");
	m_ObjCombo.AddString(L"Plant");
	m_ObjCombo.AddString(L"Tree");

	// tObjData Init
	m_tObjData.wstrObjKey = L"Obj";
	m_tObjData.iAlpha = 255;
	m_tObjData.fScale = 1.f;


	// TileInfo Dlg
	m_TileInfo.Set_Map_Tab(this);

	//return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE

	return FALSE;
}

void CMapTab::On_Select_Tile_Type()
{
	// TODO: Add your control notification handler code here
	m_iRadio = R_TILE;
	m_pMap->Show_PrevObj(false);
	UpdateData(FALSE);

	// ������Ʈ �����ϰ� �ϰ�
	vector<CObj*>* Obj_List_Ptr = m_pMap->Get_ObjVecPtr();
	for (CObj* pObj : *Obj_List_Ptr)
	{
		pObj->Set_Alpha(100);
	}

	CGraphicDev::Get_Instance()->Render_Begin();
	m_pMap->Render();
	CGraphicDev::Get_Instance()->Render_End();

	m_bySelectedTile = m_TileList.GetCurSel();
	wchar_t strImgName[MIN_STR];
	wsprintf(strImgName, L"Tile%d", m_bySelectedTile);

	auto& iter = m_MapPngImage.find(strImgName);
	m_TilePicture.SetBitmap(*iter->second);
}




void CMapTab::On_Select_Stage_Category()
{
	// TODO: Add your control notification handler code here

	int iSelect = m_MapCategoryList.GetCurSel();

	if (0 > iSelect) return;

	m_MapList.ResetContent();
	m_iStageCnt = 0;

	CString cstrCategory;
	m_MapCategoryList.GetText(iSelect, cstrCategory);

	//list<CString> StrList;
	//Insert_Obj_Texture(cstrCategory.operator LPCWSTR(), &StrList);

	//for (CString& cstr : StrList)
	//{
	//	m_MapList.AddString(cstr);
	//}

	CString tpath
		= CString(L"..\\Texture\\Stage\\")
		+ cstrCategory
		+ CString(L"\\*.*");

	//�˻� Ŭ����
	CFileFind finder;

	//CFileFind�� ����, ���͸��� �����ϸ� TRUE �� ��ȯ��
	BOOL bWorking = finder.FindFile(tpath); //

	CString fileName;
	CString DirName;

	while (bWorking)
	{
		//���� ���� / ���� �� �����ϸ�ٸ� TRUE ��ȯ
		bWorking = finder.FindNextFile();

		//���� �϶�
		/*if (finder.IsArchived())
		{*/
			//������ �̸�
			CString _fileName = finder.GetFileName();

			// �������� �������� ����������� ����
			if (_fileName == _T(".") ||
				_fileName == _T("..") ||
				_fileName == _T("Thumbs.db")) continue;

			fileName = finder.GetFileTitle();

			m_MapList.AddString(fileName);
			//�о�� ���� �̸��� ����Ʈ�ڽ��� ����

			++m_iStageCnt;
		//}
	}
}


void CMapTab::On_Select_Stage()
{
	// TODO: Add your control notification handler code here
	int iSelect = m_MapList.GetCurSel();

	if (0 > iSelect || m_iStageCnt <= iSelect)
		return;

	CString cstrStage;
	m_MapList.GetText(iSelect, cstrStage);
	m_pMap->Set_Stage(cstrStage);

	m_pMap->Release(); // Ÿ�� ���ֱ�
	m_pMap->Load_File();
	// ������
	//CGraphicDev::Get_Instance()->Render_Begin();
	m_pMap->Render();
	//CGraphicDev::Get_Instance()->Render_End();
}

void CMapTab::On_Move_Map_Scale_Slider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int iPos = m_MapScaleSlider.GetPos();
	m_fMapScale = 0.008f * iPos + 0.2f;

	wchar_t szScale[MIN_STR] = L"x";

	swprintf_s(szScale, L"x%.2f", m_fMapScale);

	SetDlgItemText(IDC_HJ_SCALE_TEXT, szScale);
	wcout << szScale << endl;

	UpdateData(FALSE);

	CGraphicDev::Get_Instance()->Render_Begin();
	m_pMap->Set_Scale(m_fMapScale);
	m_pMap->Render();
	//((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pMap->Set_Scale(m_fMapScale);
	//((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pMap->Render();
	CGraphicDev::Get_Instance()->Render_End();

	*pResult = 0;
}

void CMapTab::On_Reset_Tile()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); // ��Ʈ�� -> ������ ����
	m_pMap->Tile_Initialize();

	CGraphicDev::Get_Instance()->Render_Begin();
	m_pMap->Render();
	CGraphicDev::Get_Instance()->Render_End();
}


void CMapTab::On_Save()
{
	// TODO: Add your control notification handler code here

	CString cstrFile = L"";
	m_MapList.GetText(m_MapList.GetCurSel(), cstrFile);

	//#1. Ÿ�� ����
	{
		CString cstrPath = L"../Data/Tile/" + cstrFile + L".dat";

		HANDLE	hFile = CreateFile(
			cstrPath,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		DWORD		dwByte;
		int			iTileX = 0;
		int			iTileY = 0;
		vector<TILE*>& vecTile = m_pMap->Get_vecTile();

		m_pMap->Get_StageInfo(iTileX, iTileY);

		WriteFile(hFile, &iTileX, sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &iTileY, sizeof(int), &dwByte, NULL);

		for (auto pTile : vecTile)
		{
			WriteFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
		}

		CloseHandle(hFile);
	}

	//#2 ������Ʈ ����
	{
		CString cstrPath = L"../Data/Obj/" + cstrFile + L".dat";

		HANDLE	hFile = CreateFile(
			cstrPath,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		vector<CObj*>* pObj_Vec_Ptr = m_pMap->Get_ObjVecPtr();

		DWORD		dwByte = 0;

		for (auto pObj : *pObj_Vec_Ptr)
		{
			OBJ_DATA* pData = pObj->Get_DataPtr();
			WriteFile(hFile, pData, sizeof(OBJ_DATA), &dwByte, NULL);
		}

		CloseHandle(hFile);
	}

	MessageBox(cstrFile + L".dat �� �����߽��ϴ�!", L"Save Success", MB_OK);
}

//
//void CMapTab::On_Load()
//{
//	// TODO: Add your control notification handler code here
//
//	/*****************************************************************************/
//	//##1) ��
//	{
//		CString cstrFile = L"";
//		m_MapList.GetText(m_MapList.GetCurSel(), cstrFile);
//		CString cstrPath = L"../Data/" + cstrFile + L".dat";
//
//
//		HANDLE	hFile = CreateFile(
//			cstrPath,
//			GENERIC_READ,
//			0,
//			NULL,
//			OPEN_EXISTING,
//			FILE_ATTRIBUTE_NORMAL,
//			NULL);
//
//		vector<TILE*>& vecTile = m_pMap->Get_TileVecPtr();
//		for_each(vecTile.begin(), vecTile.end(), DeleteObj());
//		vecTile.clear();
//
//		DWORD		dwByte;
//		wchar_t		strStageName[MIN_STR];
//		int			iTileX, iTileY;
//
//		ReadFile(hFile, &iTileX, sizeof(int), &dwByte, NULL);
//		ReadFile(hFile, &iTileY, sizeof(int), &dwByte, NULL);
//
//		m_pMap->Set_StageInfo(iTileX, iTileY);
//
//		while (true)
//		{
//			TILE*		pTile = new TILE;
//
//			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
//
//			if (0 == dwByte)
//			{
//				Safe_Delete(pTile);
//				break;
//			}
//
//			vecTile.push_back(pTile);
//		}
//
//		CloseHandle(hFile);
//	}
//
//	/*****************************************************************************/
//	//##2 ������Ʈ
//
//	{
//		HANDLE		hFile = CreateFile(L"../Data/Object.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//
//		DWORD		dwByte = 0;
//
//		while (true)
//		{
//			CObj*		pWall = new CObj;
//
//
//			ReadFile(hFile, pWall, sizeof(CObj), &dwByte, NULL);
//
//
//			if (dwByte == 0)
//			{
//				Safe_Delete(pWall);
//				break;
//			}
//
//			pWall->Set_MainView(m_pMap->Get_pMainView());
//			m_vecWall.push_back(pWall);
//		}
//
//		CloseHandle(hFile);
//	}
//
//	/*****************************************************************************/
//
//
//	CGraphicDev::Get_Instance()->Render_Begin();
//	m_pMap->Render();
//	CGraphicDev::Get_Instance()->Render_End();
//
//
//}


void CMapTab::On_Reset_Obj()
{
	// TODO: Add your control notification handler code here
	m_pMap->Reset_Object();

	CGraphicDev::Get_Instance()->Render_Begin();
	m_pMap->Render();
	CGraphicDev::Get_Instance()->Render_End();
}



void CMapTab::On_Move_Alpha_Slider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	m_tObjData.iAlpha = m_AlphaSlider.GetPos();

	wchar_t szBuf[MIN_STR] = L"";
	swprintf_s(szBuf, L"%d", m_tObjData.iAlpha);
	SetDlgItemText(IDC_ALPHA_TEXT, szBuf);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMapTab::On_Move_Obj_Scale_Slider(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	int iPos = m_ObjScaleSlider.GetPos();
	m_tObjData.fScale = iPos * 0.1f;
	m_pMap->Get_pPrevObj()->Set_Scale(m_tObjData.fScale);

	wchar_t szBuf[MIN_STR] = L"x";
	swprintf_s(szBuf, L"x%.2f", m_tObjData.fScale);
	SetDlgItemText(IDC_OBJ_SCALE_TEXT, szBuf);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMapTab::On_Select_Object()
{
	// TODO: Add your control notification handler code here

	// ���� ��ư ����
	m_iRadio = R_OBJ;

	// ������Ʈ �����ϰ� �ϰ�
	vector<CObj*>* Obj_List_Ptr = m_pMap->Get_ObjVecPtr();
	for (CObj* pObj : *Obj_List_Ptr)
	{
		pObj->Set_Alpha(255);
	}

	// ��Ʈ�� �̸�����
	int		iSelectIndex = m_ObjList.GetCurSel();

	if (0 > iSelectIndex)
		return;

	CString cstrItem = L"";
	m_ObjList.GetText(iSelectIndex, cstrItem);

	auto	iter = m_MapPngImage.find(cstrItem);

	if (iter == m_MapPngImage.end())
		return;

	m_ObjPicture.SetBitmap(*iter->second);

	int		i = 0;

	for (i = 0; i < cstrItem.GetLength(); ++i)
	{
		if (0 != isdigit(cstrItem[i]))	// ���ڿ��� 0������ �˻縦 �����Ͽ� ���ڿ� �� ���� ������ �������� �Ǵ� ���� ������ ���������� �Ǻ��ϴ� �Լ�
			break;							// ��ȯ ���� 0�� �ƴ� ��찡 ���� ���·� �� ����	
	}

	cstrItem.Delete(0, i);
	m_tObjData.iCount = _tstoi(cstrItem);
	m_pMap->Get_pPrevObj()->Set_Data(m_tObjData);

	m_pMap->Show_PrevObj(true);

	UpdateData(FALSE);
}



void CMapTab::On_Select_Object_Category()
{
	// TODO: Add your control notification handler code here

	int iSelect = m_ObjCombo.GetCurSel();

	CString cstrStateKey = L"";
	m_ObjCombo.GetLBText(iSelect, cstrStateKey);
	m_tObjData.wstrStateKey = cstrStateKey;
	m_pMap->Get_pPrevObj()->Set_StateKey(cstrStateKey.operator LPCWSTR());

	m_ObjList.ResetContent();

	// Ȯ��
	list<CString> StrList;
	Insert_Obj_Texture(cstrStateKey.operator LPCWSTR(), &StrList);

	for (CString& cstr : StrList)
	{
		m_ObjList.AddString(cstr);
	}

	HorizontalScroll();
}

void CMapTab::Insert_Obj_Texture(wstring wstrStateKey, list<CString>* ItemListPtr)
{
	CString tpath
		= CString(L"..\\Texture\\Obj\\")
		+ wstrStateKey.c_str()
		+ CString(L"\\*.*");

	//�˻� Ŭ����
	CFileFind finder;
	int iObjCnt = 0;

	//CFileFind�� ����, ���͸��� �����ϸ� TRUE �� ��ȯ��
	BOOL bWorking = finder.FindFile(tpath);

	while (bWorking)
	{
		//���� ���� / ���� �� �����ϸ�ٸ� TRUE ��ȯ
		bWorking = finder.FindNextFile();

		//���� �϶�
		//if (finder.IsArchived())
		//{
			//������ �̸�
			CString fileName = finder.GetFileName();

			// �������� �������� ����������� ����
			if (fileName == _T(".") ||
				fileName == _T("..") ||
				fileName == _T("Thumbs.db")) continue;

			fileName = finder.GetFileTitle();

			auto& iter = m_MapPngImage.find(fileName);
			if (iter == m_MapPngImage.end())
			{
				CImage* pImage = new CImage;
				pImage->Load(finder.GetFilePath());
				m_MapPngImage.emplace(fileName, pImage);
			}

			if (nullptr != ItemListPtr)
				ItemListPtr->push_back(fileName);

			//m_ObjList.AddString(fileName);
			//�о�� ���� �̸��� ����Ʈ�ڽ��� ����

			++iObjCnt;
		//}
	}

	wstring wstrPath = L"../Texture/Obj/" + wstrStateKey + L"/" + wstrStateKey + L"%d.png";
	CTextureMgr::Get_Instance()->InsertTexture(
		wstrPath, L"Obj", TEX_MULTI, NULL, wstrStateKey, iObjCnt
	);
}