#include "stdafx.h"
#include "InvenWnd.h"

#include "ObjMgr.h"

#include "UIObserver.h"
#include "DataSubject.h"

CInvenWnd::CInvenWnd()
{
}


CInvenWnd::~CInvenWnd()
{
	Release();
}

void CInvenWnd::Update_World_Matrix(void)
{
}

void CInvenWnd::Update_State(void)
{
}

HRESULT CInvenWnd::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(70.f, 160.f, 0.f);
	m_vOriginalPos = m_tInfo.vPos;
	m_tFrame.wstrObjKey = L"InvenWindow";

	m_pObserver = new CUIObserver;
	CDataSubject::Get_Instance()->Subscribe(m_pObserver);

	m_pInven = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_InvenPtr();

	m_fClickTime = 0.f;

	return S_OK;
}

int CInvenWnd::Progress(void)
{
	CWndUI::Progress();
	
	if (m_bIsShow)
	{
		m_pInven = static_cast<CUIObserver*>(m_pObserver)->Get_Inventory();
		
		// Pos
		m_vSeedPos = D3DXVECTOR3(242.f, 380.f, 0.f) + m_tInfo.vPos;
		for (int i = 0; i < 12; ++i)
		{
			float fY = 25 * i + 56;
			m_vImgPos[i] = D3DXVECTOR3(24.f, fY, 0.f) + m_tInfo.vPos;

			fY = 25 * i + 51;
			m_vNamePos[i] = D3DXVECTOR3(41.f, fY, 0.f) + m_tInfo.vPos;
			m_vCountPos[i] = D3DXVECTOR3(267.f, fY, 0.f) + m_tInfo.vPos;
		}

		// Rect
		m_rcSeed = Calculate_Rect(m_vSeedPos, 80.f, 15.f, false);
		for (int i = 0; i < 12; ++i)
		{
			m_rcImg[i] = Calculate_Rect(m_vImgPos[i], 24.f, 24.f);
			m_rcName[i] = Calculate_Rect(m_vNamePos[i], 222.f, 15.f, false);
			m_rcCount[i] = Calculate_Rect(m_vCountPos[i], 52.f, 15.f, false);
		}

		// ������ ����
		int iSeed = static_cast<CUIObserver*>(m_pObserver)->Get_PlayerInfo()->iSeed;
		wsprintf(m_szSeed, L"%d", iSeed);

		//for (int i = 0; i < m_pInven->size(); ++i)
		for (int i = 0; i < 12; ++i)
		{
			if (i >= m_pInven->size())
				continue;

			if ((*m_pInven)[i] == nullptr)
				continue;

			m_wstrName[i] = (*m_pInven)[i]->Get_ItemInfo().wstrName;
			wsprintf(m_szCount[i], L"%d", (*m_pInven)[i]->Get_ItemInfo().iCount);
		}

		// �����ϱ�
		m_fClickTime += GET_TIME;

		POINT pt = GetMousePt();
		CMenuUI* pMenu = static_cast<CMenuUI*>(CObjMgr::Get_Instance()->Get_MenuUI());
		CMouse* pMouse = static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());

		for (int i = 0; i < 12; ++i)
		{
			// �������� ���� �ִ� ��Ȳ�̸� �κ��丮 �����ϱ�
			// ������ ����ϱ� (���콺 �� ����Ŭ��)
			// ������ �Ǹ��ϱ�
			if (PtInRect(&m_rcImg[i], pt) || PtInRect(&m_rcName[i], pt) || PtInRect(&m_rcCount[i], pt))
			{
				if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_INVEN_WND+10)) // ????? +10
				{
					// �κ� ����
					if (pMouse->Is_Picking_Sth())
					{
						CItem* pItem = pMouse->Get_Picking_Item();
						int iPicking_Item_Index = 0;
						for (int j = 0; j < m_pInven->size(); ++j)
						{
							if ((*m_pInven)[j] == pItem)
							{
								iPicking_Item_Index = j;
								break;
							}
						}

						if (m_pInven->size() <= i)
						{
							int iInterval = i - m_pInven->size() + 1;
							for(int j=0; j<iInterval; ++j)
								m_pInven->push_back(nullptr);
						}

						CItem* pTemp = (*m_pInven)[i];
						(*m_pInven)[i] = pItem; // Ŭ���� ��ġ�� ���� ��� �ִ� �������� ������ ����
						(*m_pInven)[iPicking_Item_Index] = pTemp; // ��� �ִ� ������ ��ġ�� 
						pMouse->End_Picking();
					}

					// ������ ���, �Ǹ�(����Ŭ�� ���)
					else
					{
						if (m_fClickTime <= 1.f)
						{
							if (i < m_pInven->size())
							{
								CItem* pItem = (*m_pInven)[i];
								if (nullptr == pItem)
									return 0;

								CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());

								if (m_bIsShopping)
									pPlayer->Sell_Item(pItem);
								else
									pPlayer->Use_Item(pItem);

								/*if (pItem->Get_ItemInfo().iCount <= 0)
								{
									m_pInven->erase(m_pInven->begin() + i);
								}*/

								if (pItem->Get_ItemInfo().eItemID == IT_POTION)
								{
									if (pItem->Get_ItemInfo().iCount <= 0)
									{
										m_pInven->erase(m_pInven->begin() + i);
									}
								}
								else
								{
									if (pItem->Get_ItemInfo().iCount == 1)
										m_pInven->erase(m_pInven->begin() + i);
									else
										pItem->Get_ItemInfo().iCount--;
								}
							}
						}
						m_fClickTime = 0.f;
					}
				}
			}

			// ������ ����(�� �� �ٴ� Ŭ���ϸ� �����ų�, �κ� ����)
			if (PtInRect(&m_rcImg[i], pt))
			{
				if (!pMouse->Is_Picking_Sth())
				{
					if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_UI_MENU))
					{
						static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse())
							->Pick((*m_pInven)[i]);
					}
				}

				return 0;
			}

			// �� ���Կ��� ������ ��� �� ������ 9�� �Ǹ� ������ â���� ���ֱ�
			if (i < m_pInven->size())
			{
				CItem* pItem = (*m_pInven)[i];
				if (pItem != nullptr)
				{
					if (pItem->Get_ItemInfo().iCount <= 0)
					{
						m_pInven->erase(m_pInven->begin() + i);
					}
				}
			}
		}
	}

	return 0;
}

void CInvenWnd::Render(void)
{
	CWndUI::Render();

	if (m_bIsShow)
	{
		// �κ��丮 ������ ǥ��
		m_pInven = static_cast<CUIObserver*>(m_pObserver)->Get_Inventory();
		for (int i = 0; i < 12; ++i)
		{
			if (i >= m_pInven->size())
				continue;

			if ((*m_pInven)[i] == nullptr)
				continue;

			//1) �׸�
			wstring wstrObjKey = (*m_pInven)[i]->Get_ItemInfo().wstrObjKey;
			const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(wstrObjKey);

			if (nullptr == pTexture)
				continue;

			float fX = float(pTexture->tImgInfo.Width) * 0.5f;
			float fY = float(pTexture->tImgInfo.Height) * 0.5f;

			D3DXMATRIX	matTrans;
			D3DXMatrixTranslation(&matTrans, m_vImgPos[i].x, m_vImgPos[i].y, 0.f);

			CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
				NULL,
				&D3DXVECTOR3(fX, fY, 0.f),
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			//2) �̸�
			D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

			CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
				CGraphicDev::Get_Instance()->GetSprite(),
				m_wstrName[i].c_str(),
				lstrlen(m_wstrName[i].c_str()),
				&m_rcName[i],
				DT_CENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			//3) ����
			CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
				CGraphicDev::Get_Instance()->GetSprite(),
				m_szCount[i],
				lstrlen(m_szCount[i]),
				&m_rcCount[i],
				DT_CENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		//# ���� ǥ��
		CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
			CGraphicDev::Get_Instance()->GetSprite(),
			m_szSeed,
			lstrlen(m_szSeed),
			&m_rcSeed,
			DT_CENTER,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		// �����
		for (int i = 0; i < 12; ++i)
		{
			CGraphicDev::Get_Instance()->Draw_Rect(m_rcImg[i]);
			CGraphicDev::Get_Instance()->Draw_Rect(m_rcName[i]);
			CGraphicDev::Get_Instance()->Draw_Rect(m_rcCount[i]);
		}
	}
}

void CInvenWnd::Release(void)
{
	Safe_Delete(m_pObserver);
}
