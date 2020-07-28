#include "stdafx.h"
#include "MainGame.h"
#include "ObjFactory.h"
#include "FileInfo_Client.h"
#include "CollisionMgr.h"
#include "MapMgr.h"
#include "KeyMgr2.h"
#include "DataSubject.h"

CMainGame::CMainGame()
	: m_pGraphicDev(CGraphicDev::Get_Instance())
	, m_pSceneMgr(CSceneMgr::Get_Instance())
	, m_pTimeMgr(CTimeMgr::Get_Instance())
	, m_fTime(0.f)
	, m_iCount(0)
{
}

CMainGame::~CMainGame()
{
	Release();
}

HRESULT CMainGame::Initialize(void)
{
	if (FAILED(m_pGraphicDev->Initialize()))
	{
		ERR_MSG(L"MainGame Init Failed");
		return E_FAIL;
	}
	
	if (FAILED(CItemMgr::Get_Instance()->Initialize()))
	{
		ERR_MSG(L"ItemMgr Init Failed");
		return E_FAIL;
	}

	m_pSceneMgr->SetScene(SC_LOGO);
	//m_pSceneMgr->SetScene(SC_MENU);
	//m_pSceneMgr->SetScene(SC_STAGE);
	CTimeMgr::Get_Instance()->Initialize();
	CMapMgr::Get_Instance()->Initialize();
	return S_OK;
}

void CMainGame::Progress(void)
{
	m_pTimeMgr->SetTime(); // CTimeMgr::m_fDirSetTime 갱신
	CKeyMgr::Get_Instance()->SetKey(); // CKeyMgr::m_dwKey 갱신

	m_pSceneMgr->Progress();

	m_fTime += GET_TIME;
	D3DXMatrixTranslation(&m_matTrans, 600.f, 50.f, 0.f);
}

void CMainGame::Late_Progress(void)
{
	m_pSceneMgr->Late_Progress();
}

void CMainGame::Render(void)
{
	++m_iCount;
	//m_fDirSetTime += GET_TIME;
	//cout << "MainGame: " << m_fDirSetTime << endl;		

	if (m_fTime > 1.f)
	{
		m_fTime = 0.f;
		wsprintf(m_szFPS, L"FPS: %d", m_iCount);
		m_iCount = 0.f;
	}

	m_pGraphicDev->GetDevice()->Clear(0, 
						NULL,
						D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 
						D3DCOLOR_ARGB(255, 0, 0, 255),	// 백버퍼 색상
						1.f,		// z버퍼의 초기화 값
						0);			// 스텐실 버퍼의 초기화 값

	m_pGraphicDev->Render_Begin();

	m_pSceneMgr->Render();
	m_pGraphicDev->GetSprite()->SetTransform(&m_matTrans);
	/*m_pGraphicDev->GetFont_Normal12()->DrawTextW(m_pGraphicDev->GetSprite()
		, m_szFPS, lstrlen(m_szFPS)
		, NULL, NULL, D3DCOLOR_ARGB(255, 255, 0, 255));*/
	m_pGraphicDev->Render_End();
	m_pGraphicDev->GetDevice()->Present(NULL, NULL, NULL, NULL);
}

void CMainGame::Release(void)
{
	CKeyMgr::Get_Instance()->Destroy_Instance();
	CKeyMgr2::Get_Instance()->Destroy_Instance();
	CFileInfo_Client::Get_Instance()->Destroy_Instance();
	CMapMgr::Get_Instance()->Destroy_Instance();
	CAstar::Get_Instance()->Destroy_Instance();
	CDataSubject::Get_Instance()->Destroy_Instance();
	CSoundMgr::Get_Instance()->Destroy_Instance();
	
	m_pTimeMgr->Destroy_Instance();
	m_pGraphicDev->Destroy_Instance();
	CItemMgr::Get_Instance()->Destroy_Instance();
	m_pSceneMgr->Destroy_Instance();
	CTextureMgr::Get_Instance()->Destroy_Instance();
	CObjMgr::Get_Instance()->Destroy_Instance();
}
