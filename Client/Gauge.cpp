#include "stdafx.h"
#include "Gauge.h"

#include "UIObserver.h"
#include "DataSubject.h"

CGauge::CGauge()
{
}


CGauge::~CGauge()
{
	Release();
}

void CGauge::Update_World_Matrix(void)
{
}

void CGauge::Update_State(void)
{
}

HRESULT CGauge::Initialize(void)
{
	m_vWndPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vHPBarPos = m_vWndPos + D3DXVECTOR3(95.f, 17.f, 0.f);
	m_vMPBarPos = m_vWndPos + D3DXVECTOR3(95.f, 32.f, 0.f);
	m_vSPBarPos = m_vWndPos + D3DXVECTOR3(95.f, 47.f, 0.f);
	m_vIconPos = m_vWndPos + D3DXVECTOR3(64.f, 83.f, 0.f);

	m_pObserver = new CUIObserver;
	CDataSubject::Get_Instance()->Subscribe(m_pObserver);

	return E_NOTIMPL;
}

int CGauge::Progress(void)
{
	m_iHp = dynamic_cast<CUIObserver*>(m_pObserver)->Get_StatusInfo()->iHp;
	m_iMp = dynamic_cast<CUIObserver*>(m_pObserver)->Get_StatusInfo()->iMp;
	m_iSp = dynamic_cast<CUIObserver*>(m_pObserver)->Get_PlayerInfo()->iSp;

	m_iMaxHp = dynamic_cast<CUIObserver*>(m_pObserver)->Get_StatusInfo()->iMaxHp;
	m_iMaxMp = dynamic_cast<CUIObserver*>(m_pObserver)->Get_StatusInfo()->iMaxMp;
	m_iMaxSp = dynamic_cast<CUIObserver*>(m_pObserver)->Get_PlayerInfo()->iMaxSp;

	m_eMove = dynamic_cast<CUIObserver*>(m_pObserver)->Get_PlayerMoveState();
	return 0;
}

void CGauge::Render(void)
{
	D3DXMATRIX	matTrans, matScale, matWorld;

	//# À©µµ¿ì
	{
		const TEXINFO* pTexture
			= CTextureMgr::Get_Instance()->GetTexture(L"GaugeWnd");

		if (nullptr == pTexture)
			return;

		D3DXMatrixTranslation(&matTrans, m_vWndPos.x, m_vWndPos.y, 0.f);

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	//# HPBar
	{
		const TEXINFO* pTexture
			= CTextureMgr::Get_Instance()->GetTexture(L"HpBar");

		if (nullptr == pTexture)
			return;

		D3DXMatrixTranslation(&matTrans, m_vHPBarPos.x, m_vHPBarPos.y, 0.f);
	
		float fScale = (float)m_iHp / m_iMaxHp;
		D3DXMatrixScaling(&matScale, fScale, 1.f, 0.f);
		
		matWorld = matScale * matTrans;
		
		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	//# MPBar
	{
		const TEXINFO* pTexture
			= CTextureMgr::Get_Instance()->GetTexture(L"MpBar");

		if (nullptr == pTexture)
			return;

		D3DXMatrixTranslation(&matTrans, m_vMPBarPos.x, m_vMPBarPos.y, 0.f);

		float fScale = (float)m_iMp / m_iMaxMp;
		D3DXMatrixScaling(&matScale, fScale, 1.f, 0.f);

		matWorld = matScale * matTrans;

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	//# SPBar
	{
		const TEXINFO* pTexture
			= CTextureMgr::Get_Instance()->GetTexture(L"SPBar");

		if (nullptr == pTexture)
			return;

		D3DXMatrixTranslation(&matTrans, m_vSPBarPos.x, m_vSPBarPos.y, 0.f);

		float fScale = (float)m_iSp / m_iMaxSp;
		D3DXMatrixScaling(&matScale, fScale, 1.f, 0.f);

		matWorld = matScale * matTrans;

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	//# Icon
	{
		const TEXINFO* pTexture = nullptr;
		
		if(m_eMove == CPlayer::MS_WALK)
			pTexture = CTextureMgr::Get_Instance()->GetTexture(L"WalkIcon");

		else if(m_eMove == CPlayer::MS_RUN)
			pTexture = CTextureMgr::Get_Instance()->GetTexture(L"RunIcon");

		else
			pTexture = CTextureMgr::Get_Instance()->GetTexture(L"WalkIcon");

		if (nullptr == pTexture)
			return;

		float fX = float(pTexture->tImgInfo.Width) * 0.5f;
		float fY = float(pTexture->tImgInfo.Height) * 0.5f;

		D3DXMatrixTranslation(&matTrans, m_vIconPos.x, m_vIconPos.y, 0.f);

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CGauge::Release(void)
{
	Safe_Delete(m_pObserver);
}
