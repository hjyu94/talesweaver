#include "stdafx.h"
#include "HPBar.h"
#include "DataSubject.h"
#include "UIObserver.h"
#include "GraphicDev.h"
#include "ObjMgr.h"

CHPBar::CHPBar()
{
	m_eRenderID = RDR_EFFECT;
}


CHPBar::~CHPBar()
{
	Release();
}

HRESULT CHPBar::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(-26.f, 50.f, 0.f);

	m_pObserver = new CUIObserver;
	CDataSubject::Get_Instance()->Subscribe(m_pObserver);

	return S_OK;
}

int CHPBar::Progress(void)
{
	m_iHP = dynamic_cast<CUIObserver*>(m_pObserver)->Get_StatusInfo()->iHp;
	m_iMaxHP = dynamic_cast<CUIObserver*>(m_pObserver)->Get_StatusInfo()->iMaxHp;

	float fScale = (float)m_iHP / m_iMaxHP;

	D3DXMATRIX		matTrans, matScale, matParent;

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y,
		0.f);

	D3DXMatrixScaling(&matScale, fScale, 1.f, 0.f);

	matParent = *dynamic_cast<CUIObserver*>(m_pObserver)->Get_ObserverMatrix();

	m_tInfo.matWorld = matScale * matTrans * matParent;

	return 0;
}

void CHPBar::Render(void)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	
	if (!pPlayer->Is_Dead())
	{
		const TEXINFO* pTexture
			= CTextureMgr::Get_Instance()->GetTexture(L"SmallHpBar");

		if (nullptr == pTexture)
			return;

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CHPBar::Release(void)
{
	Safe_Delete(m_pObserver);
}

void CHPBar::Update_World_Matrix(void)
{
}

void CHPBar::Update_State(void)
{
}
