#include "stdafx.h"
#include "MPBar.h"
#include "DataSubject.h"
#include "UIObserver.h"
#include "GraphicDev.h"
#include "ObjMgr.h"

CMPBar::CMPBar()
{
	m_eRenderID = RDR_EFFECT;
}


CMPBar::~CMPBar()
{
	Release();
}

HRESULT CMPBar::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(-26.f, 55.f, 0.f);

	m_pObserver = new CUIObserver;
	CDataSubject::Get_Instance()->Subscribe(m_pObserver);

	return S_OK;
}

int CMPBar::Progress(void)
{
	m_iMP = dynamic_cast<CUIObserver*>(m_pObserver)->Get_StatusInfo()->iMp;
	m_iMaxMP = dynamic_cast<CUIObserver*>(m_pObserver)->Get_StatusInfo()->iMaxMp;

	float fScale = (float)m_iMP / m_iMaxMP;

	D3DXMATRIX		matTrans, matParent, matScale;

	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y,
		0.f);

	D3DXMatrixScaling(&matScale, fScale, 1.f, 0.f);

	matParent = *dynamic_cast<CUIObserver*>(m_pObserver)->Get_ObserverMatrix();

	m_tInfo.matWorld = matScale * matTrans * matParent;

	return 0;
}

void CMPBar::Render(void)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());

	if (!pPlayer->Is_Dead())
	{
		const TEXINFO* pTexture
			= CTextureMgr::Get_Instance()->GetTexture(L"SmallMpBar");

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

void CMPBar::Release(void)
{
	Safe_Delete(m_pObserver);
}

void CMPBar::Update_World_Matrix(void)
{
}

void CMPBar::Update_State(void)
{
}
