#include "stdafx.h"
#include "EquipWnd.h"

#include "ObjMgr.h"

CEquipWnd::CEquipWnd()
{

}


CEquipWnd::~CEquipWnd()
{
}

void CEquipWnd::Update_World_Matrix(void)
{
}

void CEquipWnd::Update_State(void)
{
}

HRESULT CEquipWnd::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(420.f, 220.f, 0.f);
	m_vOriginalPos = m_tInfo.vPos;
	m_tFrame.wstrObjKey = L"EquipWnd";

	m_fClickTime = 0.f;
	return E_NOTIMPL;
}

int CEquipWnd::Progress(void)
{
	if (m_bIsShow)
	{

		m_InvenPtr = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_InvenPtr();
		m_EquipPtr = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_EquipPtr();


		for (int i = 0; i < EQ_END; ++i)
		{
			float fY = 31.f * i + 38.f;
			m_arrItemPos[i] = D3DXVECTOR3{ 18.f, fY, 0.f } + m_tInfo.vPos;
			m_arrItemRect[i] = Calculate_Rect(m_arrItemPos[i], 24.f, 24.f);

			fY = 31.f * i + 26.f;
			m_arrNamePos[i] = D3DXVECTOR3{ 36.f, fY, 0.f } + m_tInfo.vPos;
			m_arrNameRect[i] = Calculate_Rect(m_arrNamePos[i], 156.f, 30.f, false);
		}

		for (int i = 0; i < EQ_END; ++i)
		{
			if (nullptr != m_EquipPtr[i])
			{
				m_wstrName[i] = m_EquipPtr[i] ->Get_ItemInfo().wstrName;
			}
		}

		// 해제하기
		m_fClickTime += GET_TIME;
		POINT pt = GetMousePt();
		for (int i = 0; i < 12; ++i)
		{
			if (PtInRect(&m_arrItemRect[i], pt) || PtInRect(&m_arrNameRect[i], pt))
			{
				if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_INVEN_WND))
				{
					if (m_fClickTime <= 1.f)
					{
						CEquipment* pItem = m_EquipPtr[i];
						if (nullptr == pItem)
							return 0;
						static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->UnEqiup_Item(pItem);
					}
				
					m_fClickTime = 0.f;
				}
			}
		}
	}


	CWndUI::Progress();
	return 0;
}

void CEquipWnd::Render(void)
{
	CWndUI::Render();

	if (m_bIsShow)
	{
		for (int i = 0; i < EQ_END; ++i)
		{
			if (nullptr != m_EquipPtr[i])
			{
				//1) 그림
				wstring wstrObjKey = (m_EquipPtr[i])->Get_ItemInfo().wstrObjKey;
				const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(wstrObjKey);

				if (nullptr == pTexture)
					return;

				float fX = float(pTexture->tImgInfo.Width)  * 0.5f;
				float fY = float(pTexture->tImgInfo.Height) * 0.5f;

				D3DXMATRIX	matTrans;
				D3DXMatrixTranslation(&matTrans, m_arrItemPos[i].x, m_arrItemPos[i].y, 0.f);

				CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
				CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
					NULL,
					&D3DXVECTOR3(fX, fY, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));

				//2) 이름
				D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

				CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
				CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
					CGraphicDev::Get_Instance()->GetSprite(),
					m_wstrName[i].c_str(),
					lstrlen(m_wstrName[i].c_str()),
					&m_arrNameRect[i],
					DT_CENTER,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			CGraphicDev::Get_Instance()->Draw_Rect(m_arrItemRect[i]);
			CGraphicDev::Get_Instance()->Draw_Rect(m_arrNameRect[i]);
		}
	}
}

void CEquipWnd::Release(void)
{
}
