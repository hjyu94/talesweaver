#include "stdafx.h"
#include "EffectBuff.h"
#include "ObjMgr.h"

CEffectBuff::CEffectBuff()
{
}

CEffectBuff::CEffectBuff(wstring wstrObjKey, wstring wstrStateKey, float fMaxTime)
{
	m_tFrame.wstrObjKey = wstrObjKey;
	m_tFrame.wstrStateKey = wstrStateKey;
	m_fMaxTime = fMaxTime;
	m_fCurTime = 0;
}


CEffectBuff::~CEffectBuff()
{
}

HRESULT CEffectBuff::Initialize(void)
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

int CEffectBuff::Progress(void)
{
	CEffect::Progress();
	
	D3DXMATRIX		matTrans;
	
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
	
	m_tInfo.matWorld = matTrans * m_matParent;
	
	m_tFrame.fFrameTime += GET_TIME;
	
	if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fFrameTime = 0;
	
		if(m_tFrame.iSpriteCnt < m_tFrame.iSpriteMax-1)
			m_tFrame.iSpriteCnt++;
	}
	
	m_fCurTime += GET_TIME;

	if (m_fCurTime >= m_fMaxTime)
	{
		if (m_tFrame.wstrStateKey == L"Shield")
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
			pPlayer->Set_Shield(false);
		}
		return OBJ_DEAD;
	}

	return OBJ_NOEVENT;
}

void CEffectBuff::Render(void)
{
	CEffect::Render();
}

void CEffectBuff::Release(void)
{
}
