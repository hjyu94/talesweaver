#include "stdafx.h"
#include "Effect.h"

#include "DataSubject.h"
#include "UIObserver.h"

CEffect::CEffect()
{
	m_eRenderID = RDR_EFFECT;
}


CEffect::~CEffect()
{
	CDataSubject::Get_Instance()->UnSubscribe(m_pObserver);
	Release();
}

HRESULT CEffect::Initialize(void)
{
	m_pObserver = new CUIObserver;
	CDataSubject::Get_Instance()->Subscribe(m_pObserver);
	m_tFrame.wstrObjKey = L"Effect";
	return E_NOTIMPL;
}

int CEffect::Progress(void)
{
	m_matParent = *(static_cast<CUIObserver*>(m_pObserver)->Get_ObserverMatrix());
	return 0;
}

void CEffect::Render(void)
{
	const TEXINFO*		pTexture = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, m_tFrame.iSpriteCnt);

	if (nullptr == pTexture)
		return;

	float fX = float(pTexture->tImgInfo.Width) / 2.f;
	float fY = float(pTexture->tImgInfo.Height) / 2.f;

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_matParent);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CEffect::Release(void)
{
	Safe_Delete(m_pObserver);
}

void CEffect::Update_World_Matrix(void)
{
}

void CEffect::Update_State(void)
{
}

void CEffect::Late_Progress(void)
{
}
