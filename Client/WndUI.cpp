#include "stdafx.h"
#include "WndUI.h"
#include "ObjMgr.h"

CWndUI::CWndUI()
{
	m_bExitBtnOn = OFF;
}


CWndUI::~CWndUI()
{
}

int CWndUI::Progress(void)
{
	if (m_bIsShow)
	{
		m_rcTitle = {
			(LONG)(m_tInfo.vPos.x),
			(LONG)(m_tInfo.vPos.y),
			(LONG)(m_tInfo.vPos.x + m_fWidth),
			(LONG)(m_tInfo.vPos.y + 22)
		};
		
		m_vExitBtnPos = D3DXVECTOR3{ m_rcTitle.right - 15.f, m_rcTitle.top + 13.f, 0.f };
		m_rcExitBtn = Calculate_Rect(m_vExitBtnPos, 20.f, 15.f);

		D3DXVECTOR3 vMouse = GetMouse();
		POINT pt = { LONG(vMouse.x), LONG(vMouse.y) };

		//#0 창 클릭 시 우선순위 변경시키기
		if ((PtInRect(&m_tRect, pt)))
		{
			if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, 4))
				m_pMenu->Reset_Priority(this);
		}

		//#1 창 드래그
		if (dynamic_cast<CShopWnd*>(this) == nullptr)
		{
			if (PtInRect(&m_rcTitle, pt) && m_pMenu->Get_ActiveUI() == this)
			{
				// 이동시키기
				if (CKeyMgr::Get_Instance()->KeyPressing(KEY_LBUTTON))
				{
					// 처음 클릭
					if (!m_bDragging)
					{
						m_vDragDelta = GetMouse() - m_tInfo.vPos; // 15, 3
						m_bDragging = true;
					}
				}

				// 이동 중단
				else
				{
					m_bDragging = false;
				}
			}

			// 계속 클릭 중
			if (m_bDragging)
			{
				m_tInfo.vPos = GetMouse() - m_vDragDelta;
			}
		}
		else
		{
			if (PtInRect(&m_rcTitle, pt))
			{
				// 이동시키기
				if (CKeyMgr::Get_Instance()->KeyPressing(KEY_LBUTTON))
				{
					// 처음 클릭
					if (!m_bDragging)
					{
						m_vDragDelta = GetMouse() - m_tInfo.vPos; // 15, 3
						m_bDragging = true;
					}
				}

				// 이동 중단
				else
				{
					m_bDragging = false;
				}
			}

			// 계속 클릭 중
			if (m_bDragging)
			{
				m_tInfo.vPos = GetMouse() - m_vDragDelta;
			}
		}

		//#2 닫기
		if (PtInRect(&m_rcExitBtn, pt))
		{
			if (CKeyMgr::Get_Instance()->KeyPressing(KEY_LBUTTON))
			{
				m_bExitBtnOn = ON;
				CSoundMgr::Get_Instance()->PlaySound(L"BtnClick.wav", CSoundMgr::UI);
			}
			else
			{
				if (m_bExitBtnOn)
				{
					m_bIsShow = false;

					// Shop Wnd 가 꺼진 거라면 인벤토리도 같이 꺼져야 한다.
					if (dynamic_cast<CShopWnd*>(this) != nullptr)
					{
						CMenuUI* pMenu = static_cast<CMenuUI*>(CObjMgr::Get_Instance()->Get_MenuUI());
						pMenu->Get_UI(CMenuUI::PT_INVEN)->Show(false);
					}

					m_tInfo.vPos = m_vOriginalPos;
					CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::UI);
				}
				m_bExitBtnOn = OFF;
			}
		}

		//#3 Rect
		m_tRect = Calculate_Rect(m_tInfo.vPos, m_fWidth, m_fHeight, false);

		if (PtInRect(&m_tRect, pt))
		{
			static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse())->Set_State(CMouse::ON_UI, true);
		}
	}

	return 0;
}

void CWndUI::Render(void)
{
	if (m_bIsShow)
	{
		// Wnd
		{
			const TEXINFO* pTexture
				= CTextureMgr::Get_Instance()->GetTexture(m_tFrame.wstrObjKey);

			if (nullptr == pTexture)
				return;

			m_fWidth = float(pTexture->tImgInfo.Width);
			m_fHeight = float(pTexture->tImgInfo.Height);

			D3DXMATRIX	matTrans;
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

			CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
				NULL,
				NULL,
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			CGraphicDev::Get_Instance()->Draw_Rect(m_rcTitle);
		}

		// ExitBtn
		{
			const TEXINFO* pTexture
				= CTextureMgr::Get_Instance()->GetTexture(L"Button", L"CloseButton", m_bExitBtnOn);

			if (nullptr == pTexture)
				return;

			float fX = float(pTexture->tImgInfo.Width) *0.5f;
			float fY = float(pTexture->tImgInfo.Height) *0.5f;

			D3DXMATRIX	matTrans;
			D3DXMatrixTranslation(&matTrans, m_vExitBtnPos.x, m_vExitBtnPos.y, 0.f);

			CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
				NULL,
				&D3DXVECTOR3(fX, fY, 0.f),
				//NULL,
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			CGraphicDev::Get_Instance()->Draw_Rect(m_rcExitBtn);
		}

		//# Rect
		CGraphicDev::Get_Instance()->Draw_Rect(m_tRect);
	}
}

