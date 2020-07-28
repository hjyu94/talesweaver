#include "stdafx.h"
#include "CoolEffect.h"

CCoolEffect::CCoolEffect()
{
}

CCoolEffect::~CCoolEffect()
{
}

HRESULT CCoolEffect::Initialize(void)
{
	m_eRenderID = RDR_MOUSE;

	m_tInfo.vPos = D3DXVECTOR3{ 0.f, -3.f, 0.f };

	m_tFrame.wstrObjKey = L"Gauge";
	m_tFrame.wstrStateKey = L"Yellow";

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, 0);

	if (nullptr == pTexInfo)
		return E_FAIL;

	SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

	m_tFrame.iSpriteCnt = 0;
	m_tFrame.fFrameTime = 0;
	m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
	m_tFrame.iSpriteMax = tSprite_Info.iCount;

	return S_OK;
}

int CCoolEffect::Progress(void)
{
	D3DXMATRIX		matTrans;

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tFrame.fFrameTime += GET_TIME;

	if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fFrameTime = 0;
		m_tFrame.iSpriteCnt++;
	}

	if (m_tFrame.iSpriteCnt >= m_tFrame.iSpriteMax)
	{
		return OBJ_DEAD;
	}
	
	m_tInfo.matWorld = matTrans;

	return OBJ_NOEVENT;
}

void CCoolEffect::Render(void)
{
	const TEXINFO*	pTexture = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, m_tFrame.iSpriteCnt);

	if (nullptr == pTexture)
		return;

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
		NULL,
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CCoolEffect::Release(void)
{
}
