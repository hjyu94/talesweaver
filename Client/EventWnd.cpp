#include "stdafx.h"
#include "EventWnd.h"

#include "ObjMgr.h"

CEventWnd::CEventWnd()
{
}


CEventWnd::~CEventWnd()
{
}

HRESULT CEventWnd::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(14.f, 110.f, 0.f);
	m_vOriginalPos = m_tInfo.vPos;
	m_tFrame.wstrObjKey = L"HelpWnd";
	m_bIsShow = false;

	m_pMenu = static_cast<CMenuUI*>(CObjMgr::Get_Instance()->Get_MenuUI());
	return S_OK;
}

int CEventWnd::Progress(void)
{
	CWndUI::Progress();

	m_tRect = Calculate_Rect(m_tInfo.vPos, 237.f, 98.f, false);

	if (m_bQuestStart)
	{
		wstrQuest = L"몬스터 퇴치해주기 ";
		TCHAR szBuf[MIN_STR] = L"";
		wsprintf(szBuf, L"%d", m_iMonster);
		wstrQuest += szBuf;
		wstrQuest += L" / 10";
	}

	return 0;
}

void CEventWnd::Render(void)
{
	CWndUI::Render();

	if (m_bIsShow)
	{
		D3DXMATRIX	matTrans;
		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

		if (!m_bQuestSuccess)
		{
			CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
				CGraphicDev::Get_Instance()->GetSprite(),
				wstrQuest.c_str(),
				lstrlen(wstrQuest.c_str()),
				&m_tRect,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else
		{
			CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
			CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
				CGraphicDev::Get_Instance()->GetSprite(),
				wstrQuest.c_str(),
				lstrlen(wstrQuest.c_str()),
				&m_tRect,
				DT_CENTER | DT_VCENTER,
				D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		CGraphicDev::Get_Instance()->Draw_Rect(m_tRect);
	}
}

void CEventWnd::Update_World_Matrix(void)
{
}

void CEventWnd::Update_State(void)
{
}

void CEventWnd::Release(void)
{
}

void CEventWnd::Quest_Get(void)
{
	m_bQuestStart = true;
}

void CEventWnd::Up_MonsterKill(void)
{
	if (!m_bQuestSuccess && m_bQuestStart)
	{
		m_iMonster++;
		if (m_iMonster == 10)
		{
			m_bQuestSuccess = true;
			m_iQuestProcess = 5;
		}
	}
}

