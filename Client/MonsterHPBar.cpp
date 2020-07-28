#include "stdafx.h"
#include "MonsterHPBar.h"
#include "DataSubject.h"
#include "UIObserver.h"
#include "GraphicDev.h"


CMonsterHPBar::CMonsterHPBar()
{
	m_eRenderID = RDR_EFFECT;
}


CMonsterHPBar::~CMonsterHPBar()
{
	Release();
}

HRESULT CMonsterHPBar::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(0.f, 45.f, 0.f);

	return S_OK;
}

int CMonsterHPBar::Progress(void)
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

void CMonsterHPBar::Render(void)
{
	const TEXINFO* pTexture
		= CTextureMgr::Get_Instance()->GetTexture(L"SmallHpBar");

	if (nullptr == pTexture)
		return;

	float fX = float(pTexture->tImgInfo.Width) * 0.5f;
	float fY = float(pTexture->tImgInfo.Height) * 0.5f;

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
		NULL,
		//NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMonsterHPBar::Release(void)
{
	Safe_Delete(m_pObserver);
}

void CMonsterHPBar::Update_World_Matrix(void)
{
}

void CMonsterHPBar::Update_State(void)
{
}
