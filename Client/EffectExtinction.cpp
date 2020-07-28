#include "stdafx.h"
#include "EffectExtinction.h"


CEffectExtinction::CEffectExtinction()
{
}

CEffectExtinction::CEffectExtinction(wstring wstrObjKey, wstring wstrStateKey)
{
	m_tFrame.wstrObjKey = wstrObjKey;
	m_tFrame.wstrStateKey = wstrStateKey;
}


CEffectExtinction::~CEffectExtinction()
{
}

HRESULT CEffectExtinction::Initialize(void)
{
	CEffect::Initialize();

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

int CEffectExtinction::Progress(void)
{
	CEffect::Progress();

	D3DXMATRIX		matTrans;

	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

	m_tInfo.matWorld = matTrans * m_matParent;

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

	return OBJ_NOEVENT;
}

void CEffectExtinction::Render(void)
{
	CEffect::Render();
}

void CEffectExtinction::Release(void)
{
}
