#include "stdafx.h"
#include "Item.h"
#include "ObjMgr.h"

CItem::CItem()
{
}

CItem::CItem(ITEM_INFO tInfo)
{
	m_tItem = tInfo;
}


CItem::~CItem()
{
}

HRESULT CItem::Initialize(void)
{
	
	return E_NOTIMPL;
}

int CItem::Progress(void)
{
	// Rect
	D3DXVECTOR3 vPos = m_tInfo.vPos - m_vScroll;
	m_tRect = Calculate_Rect(vPos, 24.f, 24.f);

	// 마우스 처리
	POINT pt = GetMousePt();
	if (PtInRect(&m_tRect, pt))
	{
		static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse())->Set_State(CMouse::ON_ITEM, true);

		// 아이템 집기
		if (CKeyMgr::Get_Instance()->KeyPressed(KEY_RBUTTON, CKeyMgr::S_ITEM))
		{
			CBack* pBack = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background());
			static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Eat_item(this);
			pBack->Delete_Item(this);
			static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse())->Set_State(CMouse::ON_ITEM, false);
		}
	}

	return 0;
}

void CItem::Render(void)
{
	D3DXMATRIX	matTrans;

	const TEXINFO* pTexture
		= CTextureMgr::Get_Instance()->GetTexture(m_tItem.wstrObjKey);

	if (nullptr == pTexture)
		return;

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

	CGraphicDev::Get_Instance()->Draw_Rect(m_tRect);

}

void CItem::Release(void)
{
}

void CItem::Update_World_Matrix(void)
{
}

void CItem::Update_State(void)
{
}

void CItem::Late_Progress(void)
{
}

void CItem::Set_TextureKey(wstring wstrObjKey, wstring wstrStateKey)
{
	m_tFrame.wstrObjKey = wstrObjKey;
	m_tFrame.wstrStateKey = wstrStateKey;
}
