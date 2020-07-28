#include "stdafx.h"
#include "QuickSlot.h"
#include "ObjMgr.h"

CQuickSlot::CQuickSlot()
{
}


CQuickSlot::~CQuickSlot()
{
}

HRESULT CQuickSlot::Initialize(void)
{
	m_eSlot = SL_1;
m_tInfo.vPos = D3DXVECTOR3(61.f, WINCY - 101.f, 0.f);
m_tFrame.wstrObjKey = L"Window";
m_tFrame.wstrStateKey = L"QuickSlot";

D3DXVECTOR3	vSlot1Pos = m_tInfo.vPos + D3DXVECTOR3{ 70.f, 10.f, 0.f };
D3DXVECTOR3	vSlot2Pos = m_tInfo.vPos + D3DXVECTOR3{ 114.f, 10.f, 0.f };

m_rcSlot[SL_1] = Calculate_Rect(vSlot1Pos, m_fSlotBtn_Width, m_fSlotBtn_Height);
m_rcSlot[SL_2] = Calculate_Rect(vSlot2Pos, m_fSlotBtn_Width, m_fSlotBtn_Height);

for (int iRow = 0; iRow < 2; ++iRow)
{
	for (int iCol = 0; iCol < 4; ++iCol)
	{
		int iIndex = iRow * 4 + iCol;
		float fX = iCol * 30.f + 24.f;
		float fY = 41.f * (iRow + 1);
		m_vItemPos[iIndex] = m_tInfo.vPos + D3DXVECTOR3(fX, fY, 0.f);
	}
}

for (int i = 0; i < 8; ++i)
{
	m_rcItem[i] = Calculate_Rect(m_vItemPos[i], 30, 26);
}

for (int i = 0; i < SL_END; ++i)
{
	for (int j = 0; j < 8; ++j)
	{
		m_arrItem[i][j] = nullptr;
	}
}
return E_NOTIMPL;
}

int CQuickSlot::Progress(void)
{
	// 슬롯1, 2 변경
	POINT pt = GetMousePt();

	if (PtInRect(&m_rcSlot[SL_1], pt))
	{
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_UI_QUICK))
		{
			m_eSlot = SL_1;
			CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::UI);
		}
	}

	if (PtInRect(&m_rcSlot[SL_2], pt))
	{
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_UI_QUICK))
		{
			m_eSlot = SL_2;
			CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::UI);
		}
	}

	// 포션, 스킬 가져다 놓기
	for (int i = 0; i < 8; ++i)
	{
		if (PtInRect(&m_rcItem[i], pt))
		{
			CMouse* pMouse = static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
			if (pMouse->Is_Picking_Sth())
			{
				if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_UI_QUICK))
				{
					CItem* pItem = pMouse->Get_Picking_Item();
					if (pItem->Get_ItemInfo().eItemID == IT_POTION || pItem->Get_ItemInfo().eItemID == IT_SKILL)
					{
						m_arrItem[m_eSlot][i] = pMouse->Get_Picking_Item();
						pMouse->End_Picking();
						CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::UI);
					}
					else
					{
						pMouse->End_Picking();
						CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::UI);
					}
				}
			}
		}
	}

	// 포션이나 스킬 사용
	DWORD arrKey[8] = { KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8 };
	for (int i = 0; i < 8; ++i)
	{
		CItem* pItem = m_arrItem[m_eSlot][i];
		if (nullptr != pItem)
		{
			if (IT_POTION == pItem->Get_ItemInfo().eItemID)
			{
				if (CKeyMgr::Get_Instance()->KeyDown(arrKey[i], CKeyMgr::S_UI_QUICK))
				{
					static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Use_Item(pItem);
				}

				if (pItem->Get_ItemInfo().iCount == 0)
				{
					m_arrItem[m_eSlot][i] = nullptr;
				}
			}

			// 스킬 시전
			else if (IT_SKILL == pItem->Get_ItemInfo().eItemID)
			{
				CSkillIcon* pSkillIcon = static_cast<CSkillIcon*>(pItem);
				pSkillIcon->Update_CoolTime();

				if (CKeyMgr::Get_Instance()->KeyDown(arrKey[i], CKeyMgr::S_UI_QUICK))
				{
					if (pSkillIcon->Get_CoolTime() > 5.f)
					{
						CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
						pPlayer->Select_Skill(pItem->Get_ItemInfo().wstrObjKey);
						pSkillIcon->Reset_CoolTime();
					}
				}
			}
		}
	}

	return 0;
}

void CQuickSlot::Render(void)
{
	// UI
	const TEXINFO* pTexture
		= CTextureMgr::Get_Instance()->GetTexture(m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, m_eSlot);

	if (nullptr == pTexture)
		return;

	D3DXMATRIX	matTrans;
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
		NULL,
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));


	// Item
	for (int i = 0; i < 8; ++i)
	{
		CItem* pItem = m_arrItem[m_eSlot][i];
		if (nullptr != pItem)
		{
			// 아이콘 출력
			const TEXINFO* pTexture
				= CTextureMgr::Get_Instance()->GetTexture(pItem->Get_ItemInfo().wstrObjKey);

			if (nullptr == pTexture)
				break;

			float fCX = float(float(pTexture->tImgInfo.Width));
			float fCY = float(float(pTexture->tImgInfo.Height));

			D3DXMATRIX	matTrans;
			D3DXMatrixTranslation(&matTrans, m_vItemPos[i].x, m_vItemPos[i].y, 0.f);

			CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
				NULL,
				&D3DXVECTOR3(fCX * 0.5f, fCY *0.5f, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
			

			// 스킬인 경우 쿨타임 상황을 투명도로 표시한다.
			if (pItem->Get_ItemInfo().eItemID == IT_SKILL)
			{
				const TEXINFO* pTexture
					= CTextureMgr::Get_Instance()->GetTexture(L"CoolTime");

				if (nullptr == pTexture)
					break;

				CSkillIcon* pSkillIcon = static_cast<CSkillIcon*>(pItem);
				float fProgress = pSkillIcon->Get_CoolTime() / 5.f;
				
				if (fProgress >= 1.f) fProgress = 0.f;
				else fProgress = 1 - fProgress;

				D3DXMATRIX	matTrans, matScale, matWorld;
				
				D3DXMatrixTranslation(&matTrans
					, (float)m_rcItem[i].left
					, (float)m_rcItem[i].bottom
					, 0.f);

				D3DXMatrixScaling(&matScale, 1.f, -fProgress, 0.f);
				
				matWorld = matScale * matTrans;

				CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);
				CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
					NULL,
					NULL,
					NULL,
					D3DCOLOR_ARGB(100, 255, 255, 255));
			}

			// 아이템인 경우(스킬이 아닌 경우) 개수 출력
			if (pItem->Get_ItemInfo().eItemID != IT_SKILL)
			{
				D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
				CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);

				TCHAR szBuf[MIN_STR] = L"";
				wsprintf(szBuf, L"%d", pItem->Get_ItemInfo().iCount);

				CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
					CGraphicDev::Get_Instance()->GetSprite(),
					szBuf,
					lstrlen(szBuf),
					&m_rcItem[i],
					DT_CENTER,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}


		}
	}

	//디버깅
	CGraphicDev::Get_Instance()->Draw_Rect(m_rcSlot[SL_1]);
	CGraphicDev::Get_Instance()->Draw_Rect(m_rcSlot[SL_2]);
	for (int i = 0; i < 8; ++i)
	{
		CGraphicDev::Get_Instance()->Draw_Rect(m_rcItem[i]);
	}
}

void CQuickSlot::Release(void)
{
}

void CQuickSlot::Update_World_Matrix(void)
{
}

void CQuickSlot::Update_State(void)
{
}
