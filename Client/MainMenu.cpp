#include "stdafx.h"
#include "MainMenu.h"

#include "TextureMgr.h"
#include "FileInfo_Client.h"
#include "GraphicDev.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "MyButton.h"

#include "ObjFactory.h"
#include "CollisionMgr.h"
#include <fstream>

int CMainMenu::m_iTotalLine = 0;
int CMainMenu::m_iReadLine = 0;
bool CMainMenu::m_bLoadEnd = false;

CMainMenu::CMainMenu()
{
	m_pStartBtn = nullptr;
	m_pExitBtn = nullptr;
}


CMainMenu::~CMainMenu()
{
	Release();
}

HRESULT CMainMenu::Initialize(void)
{
	// 멀티 텍스쳐
	if (FAILED(CTextureMgr::Get_Instance()->ReadImgPath(L"../Data/ImgPath/MultiTex_Logo_Init.txt", false)))
	{
		ERR_MSG(L"Texture Load Failed In Logo Init");
		return E_FAIL;
	}

	// 싱글 텍스쳐
	if (FAILED(CTextureMgr::Get_Instance()->ReadImgPath(L"../Data/ImgPath/SingleTex_Logo_Init.txt", true)))
	{
		ERR_MSG(L"Texture Load Failed In Logo Init");
		return E_FAIL;
	}

	//# Object: Mouse, Btn
	CObjMgr::Get_Instance()->AddObject(OBJ_MOUSE, CObjFactory<CMouse>::CreateObj());
	m_pMouse = static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_ObjListPtr(OBJ_MOUSE)->front());

	InitializeCriticalSection(&m_Crt);
	m_hThread = (HANDLE)_beginthreadex(0, 0, Img_Load_Thread, this, 0, NULL);

	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Menu.mp3");

	return S_OK;
}

int CMainMenu::Progress(void)
{
	// 로딩이 끝나면 Start, Exit 버튼 만들기
	if (!m_bBtnInit && m_bLoadEnd)
	{
		m_bBtnInit = true;

		m_pStartBtn = static_cast<CMyButton*>(CObjFactory<CMyButton>::CreateObj());
		m_pStartBtn->Set_ObjKey(L"StartBtn");
		m_pStartBtn->Set_Pos(D3DXVECTOR3(WINCX*0.5f, 480.f, 0.f));

		m_pExitBtn = static_cast<CMyButton*>(CObjFactory<CMyButton>::CreateObj());
		m_pExitBtn->Set_ObjKey(L"ExitBtn");
		m_pExitBtn->Set_Pos(D3DXVECTOR3(WINCX*0.5f, 510.f, 0.f));
	}

	if (CTextureMgr::Get_Instance()->Is_Load_Completed())
	{
		// 마우스 표시
		m_pMouse->Set_State(CMouse::STATE::S_DEFAULT);

		if (CKeyMgr::Get_Instance()->KeyDown(KEY_S, CKeyMgr::S_DEFAULT))
		{
			CloseHandle(m_hThread);
			DeleteCriticalSection(&m_Crt);

			CSceneMgr::Get_Instance()->SetScene(SC_TOWN1);
			//CSceneMgr::Get_Instance()->SetScene(SC_FIELD0);
			//CSceneMgr::Get_Instance()->SetScene(SC_FIELD3);
			return 0;
		}
	}

	m_fLoadProgress = (float)m_iReadLine / (float)m_iTotalLine;

	// 마우스, 버튼
	m_pMouse->Progress();

	if (m_pExitBtn)
		m_pExitBtn->Progress();

	if (m_pStartBtn)
		m_pStartBtn->Progress();

	return 0;
}

void CMainMenu::Render(void)
{
	//#1 배경
	const TEXINFO*		pTexture = CTextureMgr::Get_Instance()->GetTexture(L"LOGO");

	if (nullptr == pTexture)
		return;

	float	fX = float(pTexture->tImgInfo.Width) / 2.f;
	float	fY = float(pTexture->tImgInfo.Height) / 2.f;

	D3DXMATRIX	matTrans;
	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	//#3 로딩 바 출력
	if (!m_bLoadEnd)
	{
		pTexture = CTextureMgr::Get_Instance()->GetTexture(L"Loading");

		if (nullptr == pTexture)
			return;

		D3DXMATRIX	matTrans, matScale, matWorld;
		D3DXMatrixTranslation(&matTrans, WINCX*0.5F, 500.f, 0.f);

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(165.f, 62.f, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		//#4 로딩 프로그레스 바 출력
		pTexture = CTextureMgr::Get_Instance()->GetTexture(L"LoadingBar");

		if (nullptr == pTexture)
			return;

		float fWidth = float(pTexture->tImgInfo.Width);
		float fHeight = float(pTexture->tImgInfo.Height);


		D3DXMatrixTranslation(&matTrans
			, WINCX*0.5F - fWidth*0.5f
			, 500.f - fHeight*0.5f
			, 0.f);

		D3DXMatrixScaling(&matScale, m_fLoadProgress, 1.f, 0.f);

		matWorld = matScale * matTrans;


		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			//&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// 버튼-> 마우스 출력
	if (m_pStartBtn)
		m_pStartBtn->Render();

	if (m_pExitBtn)
		m_pExitBtn->Render();
	m_pMouse->Render();
}

void CMainMenu::Release(void)
{
	Safe_Delete(m_pStartBtn);
	Safe_Delete(m_pExitBtn);
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlaySound(L"BtnClick.wav", CSoundMgr::UI);
}

void CMainMenu::Late_Progress(void)
{
}

size_t CMainMenu::Img_Load_Thread(void * pArg)
{
	CMainMenu*		pLogo = (CMainMenu*)pArg;

	EnterCriticalSection(&pLogo->Get_Crt());

	wstring arrFile[] = {
		L"../Data/ImgPath/MultiTex_Logo_Init.txt"
		, L"../Data/ImgPath/SingleTex_Logo_Init.txt"
		, L"../Data/ImgPath/MultiTex_Logo_Thread.txt"
		, L"../Data/ImgPath/SingleTex_Logo_Thread.txt"
	};
	
	//m_iTotalLine = CFileInfo_Client::Get_Instance()->Get_TxtFileLine(arrFile, 4) /*+ 1 LOGO Texture*/;
	
	int iLineCnt = 0;

	for (int i = 0; i < 4; ++i)
	{
		// todo
		wifstream		LoadFile;
		TCHAR			szTemp[MAX_STR] = L"";
		LoadFile.open(arrFile[i], ios::in);

		while (LoadFile.getline(szTemp, MAX_STR, '\n'))
			++iLineCnt;

		LoadFile.close();
	}
	

	m_iTotalLine = iLineCnt;

	if (FAILED(CObjMgr::Get_Instance()->Initialize()))
	{
		ERR_MSG(L"ObjMgr Failed");
		return E_FAIL;
	}

	// 멀티 텍스쳐
	if (FAILED(CTextureMgr::Get_Instance()->ReadImgPath(L"../Data/ImgPath/MultiTex_Logo_Thread.txt", false)))
	{
		ERR_MSG(L"Texture Load Failed");
		return E_FAIL;
	}

	// 싱글 텍스쳐
	if (FAILED(CTextureMgr::Get_Instance()->ReadImgPath(L"../Data/ImgPath/SingleTex_Logo_Thread.txt", true)))
	{
		ERR_MSG(L"Single Texture Load Failed");
		return E_FAIL;
	}

	// 수작업
	CTextureMgr::Get_Instance()->PersonalProcess();

	CObjMgr::Get_Instance()->AddObject(OBJ_UI, CObjFactory<CHPBar>::CreateObj());
	CObjMgr::Get_Instance()->AddObject(OBJ_UI, CObjFactory<CMPBar>::CreateObj());
	CObjMgr::Get_Instance()->AddObject(OBJ_UI, CObjFactory<CGauge>::CreateObj());

	CObjMgr::Get_Instance()->AddObject(OBJ_UI, CObjFactory<CQuickSlot>::CreateObj());
	
	CObj* pChatUI = CObjFactory<CChat>::CreateObj();
	CObjMgr::Get_Instance()->Set_ChatUI(pChatUI);
	CObjMgr::Get_Instance()->AddObject(OBJ_UI, pChatUI);


	CObj* pMenuUI = CObjFactory<CMenuUI>::CreateObj();
	CObjMgr::Get_Instance()->Set_MenuUI(pMenuUI);
	CObjMgr::Get_Instance()->AddObject(OBJ_UI, pMenuUI);

	CObjMgr::Get_Instance()->AddObject(OBJ_PLAYER, CObjFactory<CPlayer>::CreateObj());


	m_bLoadEnd = true;
	CTextureMgr::Get_Instance()->Complete_Load(true);
	CTextureMgr::Get_Instance()->Set_Name(L"로딩 완료");

	LeaveCriticalSection(&pLogo->Get_Crt());
	_endthreadex(0);

	return 0;
}
