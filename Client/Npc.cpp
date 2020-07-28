#include "stdafx.h"
#include "Npc.h"

#include "ObjFactory.h"
#include "ObjMgr.h"

CNpc::CNpc()
{
}

CNpc::CNpc(wstring wstrName)
{
	m_tFrame.wstrObjKey = L"NPC";
	m_tFrame.wstrStateKey = wstrName.c_str();
}


CNpc::~CNpc()
{
	Release();
}

HRESULT CNpc::Initialize(void)
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, 0);

	if (nullptr == pTexInfo)
		return E_FAIL;

	SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

	m_tFrame.iSpriteCnt = 0;
	m_tFrame.fFrameTime = 0;
	m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
	m_tFrame.iSpriteMax = tSprite_Info.iCount;

	m_pShopWnd = CObjFactory<CShopWnd>::CreateObj();
	CObjMgr::Get_Instance()->AddObject(OBJ_UI, m_pShopWnd);
	m_pShopWnd->Set_Owner(this);

	return S_OK;
}

int CNpc::Progress(void)
{
	POINT pt = GetMousePt();
	if (PtInRect(&m_tRect, pt))
	{
		CMouse* pMouse = static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		if (!pMouse->Is_On_UI())
		{
			if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_NPC))
			{
				if (m_tFrame.wstrStateKey == L"Linda")
				{
					int iRadNum = rand();
					if (iRadNum % 2 == 0)
						CSoundMgr::Get_Instance()->PlaySound(L"BenyaClick0.wav", CSoundMgr::NPC);
					else
						CSoundMgr::Get_Instance()->PlaySound(L"BenyaClick1.wav", CSoundMgr::NPC);
				}
				else
				{
					CSoundMgr::Get_Instance()->PlaySound(L"OpenWnd.wav", CSoundMgr::NPC);
					
				}

				if (m_tFrame.wstrStateKey != L"Quest")
				{
					m_pShopWnd->Show(true);
				}
			}

			static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse())->Set_State(CMouse::ON_NPC, true);
		}
	}
	else
	{
		static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse())->Set_State(CMouse::ON_NPC, false);
	}

	Move_Frame();

	return 0;
}

void CNpc::Render(void)
{
	D3DXMATRIX	matTrans, matScale, matWorld;

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

	if (pTexture->tToolInfo.bReverse)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	matWorld = matScale * matTrans;

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
		NULL,
		&D3DXVECTOR3(fX, fY, 0.f),
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphicDev::Get_Instance()->Draw_Rect(m_tRect);
}

void CNpc::Release(void)
{
	for (auto iter = m_vecShop.begin(); iter != m_vecShop.end(); ++iter)
	{
		if (*iter)
		{
			delete *iter;
			*iter = nullptr;
		}
	}
	m_vecShop.clear();
}

void CNpc::Late_Progress(void)
{
}
