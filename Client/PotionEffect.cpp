#include "stdafx.h"
#include "PotionEffect.h"


CPotionEffect::CPotionEffect()
{
}

CPotionEffect::CPotionEffect(POTIONID eID)
{
	m_ePotionID = eID;
}


CPotionEffect::~CPotionEffect()
{
}

HRESULT CPotionEffect::Initialize(void)
{
	CEffect::Initialize();
	
	m_tFrame.wstrObjKey = L"Potion";

	switch (m_ePotionID)
	{
	case HP:
		m_tFrame.wstrStateKey = L"HpPotion";
		break;
	case MP:
		m_tFrame.wstrStateKey = L"MpPotion";
		break;
	case SP:
		m_tFrame.wstrStateKey = L"SoPotion";
		break;
	}


	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, 0);

	if (nullptr == pTexInfo)
		return E_FAIL;

	SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

	m_tFrame.iSpriteCnt = 0;
	m_tFrame.fFrameTime = 0;
	m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
	m_tFrame.iSpriteMax = tSprite_Info.iCount;

	return E_NOTIMPL;
}

int CPotionEffect::Progress(void)
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

void CPotionEffect::Render(void)
{
	CGraphicDev::Get_Instance()->Render_End();
	CGraphicDev::Get_Instance()->Render_Begin();

	CGraphicDev::Get_Instance()->GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	CGraphicDev::Get_Instance()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	CGraphicDev::Get_Instance()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CEffect::Render();

	CGraphicDev::Get_Instance()->Render_End();
	CGraphicDev::Get_Instance()->Render_Begin();
}

void CPotionEffect::Release(void)
{
}

void CPotionEffect::Update_World_Matrix(void)
{
}
