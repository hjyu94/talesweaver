#include "stdafx.h"
#include "Logo.h"
#include "SceneMgr.h"

CLogo::CLogo()
{
}


CLogo::~CLogo()
{
}

HRESULT CLogo::Initialize(void)
{
	SPRITE_INFO tInfo = {};
	CTextureMgr::Get_Instance()->InsertTexture(L"..\\Texture\\Loading\\Loading_%d.png", L"LOADING", TEX_MULTI, tInfo, L"LOADING", 68);
	m_iCount = 0;
	m_fTime = 0.f;
	m_bMovieEnd = false;
	//CSoundMgr::Get_Instance()->PlayBGM(L"Logo.wav");

	ShowCursor(FALSE);

	return E_NOTIMPL;
}

int CLogo::Progress(void)
{
	m_fTime += GET_TIME;

	if (!m_bInit)
	{
		m_bInit = true;
		CSoundMgr::Get_Instance()->PlaySound(L"nexon.mp3", CSoundMgr::UI);
	}
	
	switch (m_iCount)
	{
	case 0:
		if (m_fTime > 4.f)
		{
			++m_iCount;
			m_fTime = 0.f;
		}
		break;

	case 1:
		if (m_fTime > 2.f)
		{
			++m_iCount;
			m_fTime = 0.f;
		}
		break;

	case 67:
		m_bMovieEnd = true;
		CSceneMgr::Get_Instance()->SetScene(SC_MENU);
		return 0;
		break;

	default:
		if (m_fTime > 0.05f)
		{
			++m_iCount;
			m_fTime = 0.f;
		}
		break;
	}

	return 0;
}

void CLogo::Render(void)
{
	const TEXINFO*		pTexture = CTextureMgr::Get_Instance()->GetTexture(L"LOADING", L"LOADING", m_iCount);

	if (nullptr == pTexture)
		return;
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(
		pTexture->pTexture,
		NULL,
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CLogo::Release(void)
{
	CSoundMgr::Get_Instance()->StopAll();
}

void CLogo::Late_Progress(void)
{
	m_pObjMgr->Late_Progress();
}
