#include "stdafx.h"
#include "Gate.h"


CGate::CGate()
{
}


CGate::~CGate()
{
}

HRESULT CGate::Initialize(void)
{
	m_tFrame.wstrObjKey = L"WarpGate";
	m_tFrame.wstrStateKey = L"WarpGate";
	
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

int CGate::Progress(void)
{
	Move_Frame();
	return 0;
}

void CGate::Render(void)
{
	D3DXMATRIX	matTrans;

	const TEXINFO* pTexture
		= CTextureMgr::Get_Instance()->GetTexture(m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, m_tFrame.iSpriteCnt);

	if (nullptr == pTexture)
		return;

	D3DXVECTOR3 vPos = m_tInfo.vPos - m_vScroll;
	m_tRect = Calculate_Rect(vPos, float(pTexture->tImgInfo.Width), float(pTexture->tImgInfo.Height));

	D3DXMatrixTranslation(&matTrans
		, m_tInfo.vPos.x - m_vScroll.x
		, m_tInfo.vPos.y - m_vScroll.y
		, 0.f);

	float fX = float(pTexture->tImgInfo.Width) * 0.5f;
	float fY = float(pTexture->tImgInfo.Height) * 0.5f;

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CGate::Release(void)
{
}

void CGate::Update_World_Matrix(void)
{
}

void CGate::Update_State(void)
{
}

void CGate::Late_Progress(void)
{
}
