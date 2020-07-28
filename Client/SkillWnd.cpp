#include "stdafx.h"
#include "SkillWnd.h"
#include "ObjMgr.h"

CSkillWnd::CSkillWnd()
{
}


CSkillWnd::~CSkillWnd()
{
	Release();
}

void CSkillWnd::Update_World_Matrix(void)
{
}

void CSkillWnd::Update_State(void)
{
}

HRESULT CSkillWnd::Initialize(void)
{
	m_vOriginalPos = m_tInfo.vPos;
	m_tInfo.vPos = D3DXVECTOR3(360.f, 400.f, 0.f);
	m_tFrame.wstrObjKey = L"SkillWnd";


	wstring wstrSkill[] = {
		L"BotFire", L"Healing", L"Shield", L"MultiAttack", L"PurpleFog"
		,L"ThunderBolt",L"HorCut0",L"Slash",L"Freeze",L"SonicWaveIcon", 
		L"StarBustStreamIcon", L"Active2", L"Bash", L"VerCut0", 
	};

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			int iIndex = i * 10 + j;
			if (iIndex < 14)
				m_wstrSkill[i][j] = new CSkillIcon(wstrSkill[iIndex]);
			else
				m_wstrSkill[i][j] = nullptr;
		}
	}

	return S_OK;
}

int CSkillWnd::Progress(void)
{
	CWndUI::Progress();

	if (m_bIsShow)
	{
		POINT pt = GetMousePt();

		for (int i = 0; i < 3; ++i)
		{
			float fY = float(31 * i + 33);

			for (int j = 0; j < 10; ++j)
			{
				float fX = float(31 * j + 21);
				m_vSkillPos[i][j] = D3DXVECTOR3{ fX, fY, 0.f } +m_tInfo.vPos;
				m_vSkillRect[i][j] = Calculate_Rect(m_vSkillPos[i][j], 24, 24, false);

				if (PtInRect(&m_vSkillRect[i][j], pt))
				{
					if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_UI_MENU))
					{
						CItem* pItem = m_wstrSkill[i][j];
						if (nullptr != pItem)
						{
							static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse())->Pick(pItem);
						}
					}

					return 0;
				}
			}
		}
	}
	return 0;
}

void CSkillWnd::Render(void)
{
	CWndUI::Render();

	if (m_bIsShow)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				CGraphicDev::Get_Instance()->Draw_Rect(m_vSkillRect[i][j]);
			}
		}

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				
				CItem* pItem = m_wstrSkill[i][j];
				
				if (nullptr == pItem)
					break;

				wstring wstrObjKey = m_wstrSkill[i][j]->Get_ItemInfo().wstrObjKey;
				const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(wstrObjKey);

				if (nullptr == pTexture)
					continue;

				D3DXMATRIX	matTrans;
				D3DXMatrixTranslation(&matTrans, m_vSkillPos[i][j].x, m_vSkillPos[i][j].y, 0.f);

				CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
				CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
					NULL,
					NULL,
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}
}

void CSkillWnd::Release(void)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (m_wstrSkill[i][j])
			{
				delete m_wstrSkill[i][j];
				m_wstrSkill[i][j] = nullptr;
			}
		}
	}
}
