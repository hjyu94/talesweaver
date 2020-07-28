#include "stdafx.h"
#include "Etc.h"
#include "ObjMgr.h"

CEtc::CEtc()
{
}

CEtc::CEtc(wstring wstrStateKey, D3DXVECTOR3 vPos, int iCount)
{
	m_tFrame.wstrStateKey = wstrStateKey;
	m_tFrame.iSpriteCnt = iCount;
	m_tInfo.vPos = vPos;
}


CEtc::~CEtc()
{
}

HRESULT CEtc::Initialize(void)
{
	m_iAlpha = 255;
	return E_NOTIMPL;
}

int CEtc::Progress(void)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	pPlayer->Get_Info().vPos;
	pPlayer->Get_Rect();

	RECT rc = {};
	if (IntersectRect(&rc, &pPlayer->Get_Rect(), &this->Get_Rect()))
	{
		this->m_iAlpha = 150;
		pPlayer->Collision(true);
	}
	else
	{
		this->m_iAlpha = 255;
	}
	return 0;
}

void CEtc::Late_Progress(void)
{
}

void CEtc::Render(void)
{
	const TEXINFO* pTexture
		= CTextureMgr::Get_Instance()->GetTexture(L"Obj", m_tFrame.wstrStateKey, m_tFrame.iSpriteCnt);

	if (nullptr == pTexture)
		return;

	D3DXMATRIX	matTrans;
	D3DXMatrixTranslation(&matTrans
		, m_tInfo.vPos.x - m_vScroll.x
		, m_tInfo.vPos.y - m_vScroll.y
		, 0.f);

	float fCX = pTexture->tImgInfo.Width;
	float fCY = pTexture->tImgInfo.Height;

	D3DXVECTOR3 vLeftTop = m_tInfo.vPos - m_vScroll;
	vLeftTop.x -= fCX * 0.5f;
	vLeftTop.y -= fCY * 0.5f;
	
	m_tRect = Calculate_Rect(vLeftTop, fCX, fCY - 100.f, false);


	CGraphicDev::Get_Instance()->Draw_Rect(m_tRect);

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
		NULL,
		&D3DXVECTOR3(fCX*0.5f, fCY*0.5f, 0.f),
		NULL,
		D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));
}

void CEtc::Release(void)
{
}
