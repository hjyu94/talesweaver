#include "stdafx.h"
#include "ShopWnd.h"

#include "ObjMgr.h"

CShopWnd::CShopWnd()
{
}


CShopWnd::~CShopWnd()
{
}

HRESULT CShopWnd::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(50.f, 100.f, 0.f);
	m_vOriginalPos = m_tInfo.vPos;
	m_tFrame.wstrObjKey = L"ShopWnd";
	m_bIsShow = false;

	m_pMenu = static_cast<CMenuUI*>(CObjMgr::Get_Instance()->Get_MenuUI());

	
	return S_OK;
}

int CShopWnd::Progress(void)
{
	CWndUI::Progress();

	if (m_bIsShow)
	{
		// 인벤창 뜨게 하기
		CMenuUI* pMenu = static_cast<CMenuUI*>(CObjMgr::Get_Instance()->Get_MenuUI());
		for (int i = 0; i < CMenuUI::PT_END; ++i)
		{
			if (i == CMenuUI::PT_INVEN)
			{
				CWndUI* pPlayerInvenWnd = pMenu->Get_UI(CMenuUI::PT_INVEN);
				pPlayerInvenWnd->Show(true);
				pPlayerInvenWnd->Set_Pos(D3DXVECTOR3(400.f, 100.f, 0.f));
				pPlayerInvenWnd->IsShopping(true);
			}
			else
			{
				CWndUI* pWnd = pMenu->Get_UI((CMenuUI::PART)i);
				pWnd->Show(false);
			}
		}

		// 위치, 렉트
		for (int i = 0; i < 12; ++i)
		{
			float fY = 25.f * i + 54.f;
			m_vImgPos[i] = D3DXVECTOR3(23.f, fY, 0.f) + m_tInfo.vPos;
			
			fY = 25.f * i + 49.f;
			m_vNamePos[i] = D3DXVECTOR3(30.f, fY, 0.f) + m_tInfo.vPos;
			m_vCostPos[i] = D3DXVECTOR3(266.f, fY, 0.f) + m_tInfo.vPos;
			
			m_ImgRect[i] = Calculate_Rect(m_vImgPos[i], 24.f, 24.f);
			m_NameRect[i] = Calculate_Rect(m_vNamePos[i], 221.f, 15.f, false);
			m_CostRect[i] = Calculate_Rect(m_vCostPos[i], 52.f, 15.f, false);
		}

		m_vecScroll[S_UP] = D3DXVECTOR3{ 327.f, 60.f - 6.f, 0.f }	 +m_tInfo.vPos;
		m_vecScroll[S_SCROLL_MIN] = D3DXVECTOR3{ 327.f, 75.f, 0.f }	 +m_tInfo.vPos;
		m_vecScroll[S_SCROLL_MAX] = D3DXVECTOR3{ 327.f, 314.f, 0.f } +m_tInfo.vPos;
		m_vecScroll[S_DOWN] = D3DXVECTOR3{ 327.f, 327.f + 6.f, 0.f } +m_tInfo.vPos;

		m_rcScroll[S_UP] = Calculate_Rect(m_vecScroll[S_UP], 12.f, 13.f);
		m_rcScroll[S_DOWN] = Calculate_Rect(m_vecScroll[S_DOWN], 12.f, 13.f);

		// 스크롤 표시 여부
		if (m_pShopInven->size() > 12)
		{
			m_bScrollNeed = true;
		}
		else
		{
			m_bScrollNeed = false;
		}
		
		// 스크롤 위 아래 버튼 클릭 시
		POINT pt = GetMousePt();

		if (m_bScrollNeed)
		{
			if (PtInRect(&m_rcScroll[S_UP], pt))
			{
				if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_SHOP_WND))
				{
					m_iScrollStart--;
					if (m_iScrollStart < 0) m_iScrollStart = 0;
					CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::UI);
				}
			}

			if (PtInRect(&m_rcScroll[S_DOWN], pt))
			{
				if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_SHOP_WND))
				{
					m_iScrollStart++;
					if (m_iScrollStart > m_pShopInven->size() - 12) 
						m_iScrollStart = m_pShopInven->size() - 12;
					CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::UI);
				}
			}
		}

		// 물건 구매
		for (int i = 0; i < 12; ++i)
		{
			// 아이템 사용하기 (마우스 좌 더블클릭)
			// 아이템 판매하기
			if (PtInRect(&m_ImgRect[i], pt) || PtInRect(&m_NameRect[i], pt))
			{
				if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_SHOP_WND))
				{
					if (m_fClickTime <= 1.f)
					{
						CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
						pPlayer->Buy_Item((*m_pShopInven)[i + m_iScrollStart]);
					}
					m_fClickTime = 0.f;
				}
			}
		}
		//for (int i = 0; i < 12; ++i)
		//{
		//	if (PtInRect(&m_ImgRect[i], pt) || PtInRect(&m_NameRect[i], pt))
		//	{
		//		CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
		//		pPlayer->Buy_Item((*m_pShopInven)[i + m_iScrollStart]);
		//	}
		//}
	}

	else
	{
		CMenuUI* pMenu = static_cast<CMenuUI*>(CObjMgr::Get_Instance()->Get_MenuUI());
		for (int i = 0; i < CMenuUI::PT_END; ++i)
		{
			CWndUI* pWnd = pMenu->Get_UI((CMenuUI::PART)i);
			pWnd->IsShopping(false);
		}
	}

	return 0;
}

void CShopWnd::Render(void)
{
	CWndUI::Render();

	if (m_bIsShow)
	{
		for (size_t i = m_iScrollStart; i < m_iScrollStart + 12; ++i)
		{
			if (i < m_pShopInven->size())
			{
				//1) 그림
				wstring wstrObjKey = (*m_pShopInven)[i]->Get_ItemInfo().wstrObjKey;
				const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(wstrObjKey);

				if (nullptr == pTexture)
					return;

				float fX = float(pTexture->tImgInfo.Width) * 0.5f;
				float fY = float(pTexture->tImgInfo.Height) * 0.5f;

				D3DXMATRIX	matTrans;
				D3DXMatrixTranslation(&matTrans
					, m_vImgPos[i - m_iScrollStart].x
					, m_vImgPos[i - m_iScrollStart].y
					, 0.f);

				CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
				CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
					NULL,
					&D3DXVECTOR3(fX, fY, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));

				//2) 이름
				D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
				wstring wstrName = (*m_pShopInven)[i]->Get_ItemInfo().wstrName;
				CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
				CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
					CGraphicDev::Get_Instance()->GetSprite(),
					wstrName.c_str(),
					lstrlen(wstrName.c_str()),
					&m_NameRect[i - m_iScrollStart],
					DT_CENTER,
					D3DCOLOR_ARGB(255, 255, 255, 255));

				//3) 가격
				int iCost = (*m_pShopInven)[i]->Get_ItemInfo().iCost;
				TCHAR szBuf[MIN_STR] = L"";
				wsprintf(szBuf, L"%d", iCost);
				CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
					CGraphicDev::Get_Instance()->GetSprite(),
					szBuf,
					lstrlen(szBuf),
					&m_CostRect[i - m_iScrollStart],
					DT_CENTER,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}

		// 스크롤
		if (m_bScrollNeed)
		{
			// Up
			{
				const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(L"UpScroll");

				if (nullptr == pTexture)
					return;

				float fX = float(pTexture->tImgInfo.Width) * 0.5f;
				float fY = float(pTexture->tImgInfo.Height) * 0.5f;

				D3DXMATRIX	matTrans;
				D3DXMatrixTranslation(&matTrans, m_vecScroll[S_UP].x, m_vecScroll[S_UP].y, 0.f);

				CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
				CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
					NULL,
					&D3DXVECTOR3(fX, fY, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			// Down
			{
				const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(L"DownScroll");

				if (nullptr == pTexture)
					return;

				float fX = float(pTexture->tImgInfo.Width) * 0.5f;
				float fY = float(pTexture->tImgInfo.Height) * 0.5f;

				D3DXMATRIX	matTrans;
				D3DXMatrixTranslation(&matTrans, m_vecScroll[S_DOWN].x, m_vecScroll[S_DOWN].y, 0.f);

				CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
				CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
					NULL,
					&D3DXVECTOR3(fX, fY, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			// Scroll
			{
				const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(L"Scroll");

				if (nullptr == pTexture)
					return;

				float fCX = float(pTexture->tImgInfo.Width) * 0.5f;
				float fCY = float(pTexture->tImgInfo.Height) * 0.5f;

				float fIntervalY = m_vecScroll[S_SCROLL_MAX].y - m_vecScroll[S_SCROLL_MIN].y;
				int iIntervalCnt = m_pShopInven->size() - 12;

				fIntervalY = fIntervalY / iIntervalCnt;
				
				float fY = m_vecScroll[S_SCROLL_MIN].y + fIntervalY * m_iScrollStart;

				D3DXMATRIX	matTrans;
				D3DXMatrixTranslation(&matTrans
					, m_vecScroll[S_SCROLL_MIN].x
					, fY
					, 0.f);

				CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
				CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
					NULL,
					&D3DXVECTOR3(fCX, fCY, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}

		for (int i = 0; i < 12; ++i)
		{
			CGraphicDev::Get_Instance()->Draw_Rect(m_ImgRect[i]);
			CGraphicDev::Get_Instance()->Draw_Rect(m_NameRect[i]);
			CGraphicDev::Get_Instance()->Draw_Rect(m_CostRect[i]);
		}

		CGraphicDev::Get_Instance()->Draw_Rect(m_rcScroll[S_UP]);
		CGraphicDev::Get_Instance()->Draw_Rect(m_rcScroll[S_DOWN]);
	}
}

void CShopWnd::Update_World_Matrix(void)
{
}

void CShopWnd::Update_State(void)
{
}

void CShopWnd::Release(void)
{
}

void CShopWnd::Set_Owner(CNpc * pNpc)
{
	m_pShopInven = pNpc->Get_InvenPtr();
}
