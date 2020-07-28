#include "stdafx.h"
#include "StatusWnd.h"
#include "DataSubject.h"
#include "UIObserver.h"
#include "MenuUI.h"
#include "ObjMgr.h"

CStatusWnd::CStatusWnd()
{
	for (int i = 0; i < 24; ++i)
	{
		lstrcpy(m_szData[i], L"");
	}
}


CStatusWnd::~CStatusWnd()
{
	Release();
}

void CStatusWnd::Update_World_Matrix(void)
{
}

void CStatusWnd::Update_State(void)
{
}

HRESULT CStatusWnd::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(130.f, 170.f, 0.f);
	m_vOriginalPos = m_tInfo.vPos;
	m_tFrame.wstrObjKey = L"StatusWnd";

	m_pObserver = new CUIObserver;
	CDataSubject::Get_Instance()->Subscribe(m_pObserver);
	
	return E_NOTIMPL;
}

int CStatusWnd::Progress(void)
{
	if (m_bIsShow)
	{
		m_tStatus = *dynamic_cast<CUIObserver*>(m_pObserver)->Get_StatusInfo();
		m_pPlayerInfo = dynamic_cast<CUIObserver*>(m_pObserver)->Get_PlayerInfo();

		unsigned int*	arrStatus[7] = {
			&m_pPlayerInfo->iStab, &m_pPlayerInfo->iHack, &m_pPlayerInfo->iInt, &m_pPlayerInfo->iDef,
			&m_pPlayerInfo->iMr, &m_pPlayerInfo->iDex, &m_pPlayerInfo->iAgi
		};


		// 레벨, 이름
		m_vPos[0] = D3DXVECTOR3(207.f, 36.f, 0.f) + m_tInfo.vPos;
		m_rcData[0] = Calculate_Rect(m_vPos[0], 30.f, 14.f, false);

		m_vPos[1] = D3DXVECTOR3(207.f, 56.f, 0.f) + m_tInfo.vPos;
		m_rcData[1] = Calculate_Rect(m_vPos[1], 126.f, 14.f, false);

		// 칭호(추가함)
		m_vNickNamePos = D3DXVECTOR3(207.f, 76.f, 0.f) + m_tInfo.vPos;
		m_rcNickName = Calculate_Rect(m_vNickNamePos, 126.f, 14.f, false);

		// 경험치
		m_vPos[2] = D3DXVECTOR3(65.f, 135.f, 0.f) + m_tInfo.vPos;
		m_rcData[2] = Calculate_Rect(m_vPos[2], 40.f, 14.f, false);

		m_vPos[3] = D3DXVECTOR3(166, 135.f, 0.f) + m_tInfo.vPos;
		m_rcData[3] = Calculate_Rect(m_vPos[3], 40.f, 14.f, false);

		m_vPos[4] = D3DXVECTOR3(268, 135.f, 0.f) + m_tInfo.vPos;
		m_rcData[4] = Calculate_Rect(m_vPos[4], 40.f, 14.f, false);

		// 스텟 왼쪽 7개
		m_vPos[5] = D3DXVECTOR3(45.f, 183.f, 0.f) + m_tInfo.vPos;
		m_vPos[6] = D3DXVECTOR3(45.f, 183.f + 15.f * 1, 0.f) + m_tInfo.vPos;
		m_vPos[7] = D3DXVECTOR3(45.f, 183.f + 15.f * 2, 0.f) + m_tInfo.vPos;
		m_vPos[8] = D3DXVECTOR3(45.f, 183.f + 15.f * 3, 0.f) + m_tInfo.vPos;
		m_vPos[9] = D3DXVECTOR3(45.f, 183.f + 15.f * 4, 0.f) + m_tInfo.vPos;
		m_vPos[10] = D3DXVECTOR3(45.f, 183.f + 15.f * 5, 0.f) + m_tInfo.vPos;
		m_vPos[11] = D3DXVECTOR3(45.f, 183.f + 15.f * 6, 0.f) + m_tInfo.vPos;

		for (int i = 5; i <= 11; ++i)
		{
			m_rcData[i] = Calculate_Rect(m_vPos[i], 63.f, 13.f, false);
		}

		// 스텟 올리는데 필요한 포인트
		m_vPos[17] = D3DXVECTOR3(129.f, 183.f, 0.f) + m_tInfo.vPos;
		m_vPos[18] = D3DXVECTOR3(129.f, 183.f + 15.f * 1, 0.f) + m_tInfo.vPos;
		m_vPos[19] = D3DXVECTOR3(129.f, 183.f + 15.f * 2, 0.f) + m_tInfo.vPos;
		m_vPos[20] = D3DXVECTOR3(129.f, 183.f + 15.f * 3, 0.f) + m_tInfo.vPos;
		m_vPos[21] = D3DXVECTOR3(129.f, 183.f + 15.f * 4, 0.f) + m_tInfo.vPos;
		m_vPos[22] = D3DXVECTOR3(129.f, 183.f + 15.f * 5, 0.f) + m_tInfo.vPos;
		m_vPos[23] = D3DXVECTOR3(129.f, 183.f + 15.f * 6, 0.f) + m_tInfo.vPos;

		for (int i = 17; i <= 23; ++i)
		{
			m_rcData[i] = Calculate_Rect(m_vPos[i], 32.f, 15.f, false);
		}

		// 오른쪽 스텟 5개
		m_vPos[12] = D3DXVECTOR3(207.f, 183.f + 15.f * 0, 0.f) + m_tInfo.vPos;
		m_vPos[13] = D3DXVECTOR3(207.f, 183.f + 15.f * 1, 0.f) + m_tInfo.vPos;
		m_vPos[14] = D3DXVECTOR3(207.f, 183.f + 15.f * 2, 0.f) + m_tInfo.vPos;
		m_vPos[15] = D3DXVECTOR3(207.f, 183.f + 15.f * 3, 0.f) + m_tInfo.vPos;
		m_vPos[16] = D3DXVECTOR3(207.f, 183.f + 15.f * 4, 0.f) + m_tInfo.vPos;

		for (int i = 12; i <= 16; ++i)
		{
			m_rcData[i] = Calculate_Rect(m_vPos[i], 130.f, 14.f, false);
		}

		// 레벨, 이름
		wsprintf(m_szData[0], L"%d", m_pPlayerInfo->iLevel);
		lstrcpy(m_szData[1], m_pPlayerInfo->wstrName.c_str());

		// 경험치
		wsprintf(m_szData[2], L"%d", m_pPlayerInfo->iExp);
		wsprintf(m_szData[3], L"%d", m_pPlayerInfo->iLevelExp);
		wsprintf(m_szData[4], L"%d", m_pPlayerInfo->iMaxExp);

		// 왼쪽 스탯 7개
		wsprintf(m_szData[5], L"%d", m_pPlayerInfo->iStab);
		wsprintf(m_szData[6], L"%d", m_pPlayerInfo->iHack);
		wsprintf(m_szData[7], L"%d", m_pPlayerInfo->iInt);
		wsprintf(m_szData[8], L"%d", m_pPlayerInfo->iDef);
		wsprintf(m_szData[9], L"%d", m_pPlayerInfo->iMr);
		wsprintf(m_szData[10], L"%d", m_pPlayerInfo->iDex);
		wsprintf(m_szData[11], L"%d", m_pPlayerInfo->iAgi);

		// 오른쪽 스탯 7개
		wsprintf(m_szData[12], L"%d / %d", m_tStatus.iHp, m_tStatus.iMaxHp);
		wsprintf(m_szData[13], L"%d / %d", m_tStatus.iMp, m_tStatus.iMaxMp);
		wsprintf(m_szData[14], L"%d / %d", m_pPlayerInfo->iSp, m_pPlayerInfo->iMaxSp);
		wsprintf(m_szData[15], L"%d", m_pPlayerInfo->iSeed);
		wsprintf(m_szData[16], L"%d", m_pPlayerInfo->iPoint);


		// 스텟 올리는데 필요한 포인트
		for (int i = 0; i < 7; ++i)
		{
			m_iPoint[i] = *arrStatus[i] * 0.1f + 1;
			wsprintf(m_szData[17 + i], L"%d", m_iPoint[i]);
		}

		m_vExpBarPos = m_tInfo.vPos + D3DXVECTOR3(34.f, 147.f, 0.f);
		m_vMaxBtnPos = m_tInfo.vPos + D3DXVECTOR3(247.f, 37.f, 0.f);
		m_rcMaxBtn = Calculate_Rect(m_vMaxBtnPos, 27.f, 14.f, false);

		//# 키 입력
		if (m_pMenu->Get_ActiveUI() == this)
		{
			// Plus 버튼
			POINT pt = GetMousePt();

			for (int i = 0; i < 7; ++i)
			{
				m_vPlus[i] = D3DXVECTOR3(118.f, 180.f + 15.f * i, 0.f) + m_tInfo.vPos;
				m_rcPlus[i] = Calculate_Rect(m_vPlus[i], 12.f, 12.f, false);

				if (PtInRect(&m_rcPlus[i], pt))
				{
					if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_STATUS_WND))
					{
						if (m_pPlayerInfo->iPoint >= m_iPoint[i])
						{
							m_pPlayerInfo->iPoint -= m_iPoint[i];
							(*arrStatus[i])++;
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
							CSoundMgr::Get_Instance()->PlaySound(L"lvup.MP3", CSoundMgr::UI);
						}
					}
				}
			}

			// 레벨업 시키기
			if (PtInRect(&m_rcMaxBtn, pt))
			{
				if (m_pPlayerInfo->iExp >= m_pPlayerInfo->iLevelExp)
				{
					if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_STATUS_WND))
					{
						CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
						pPlayer->Level_Up(false);
					}
				}
			}
		}

		CWndUI::Progress();
	}

	return 0;
}

void CStatusWnd::Render(void)
{
	CWndUI::Render();

	if (m_bIsShow)
	{
		D3DXMATRIX matTrans, matScale, matWorld;
		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);

		for (int i = 0; i < 24; ++i)
		{
			CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
				CGraphicDev::Get_Instance()->GetSprite(),
				m_szData[i],
				lstrlen(m_szData[i]),
				&m_rcData[i],
				DT_CENTER,
				D3DCOLOR_ARGB(255, 255, 255, 255));

			CGraphicDev::Get_Instance()->Draw_Rect(m_rcData[i]);
		}

		TCHAR szData[] = L"지옥에서 온";

		CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
			CGraphicDev::Get_Instance()->GetSprite(),
			szData,
			lstrlen(szData),
			&m_rcNickName,
			DT_CENTER,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		CGraphicDev::Get_Instance()->Draw_Rect(m_rcNickName);

			
		// ExpBar
		int iExp = m_pPlayerInfo->iExp;
		int iLevelExp = m_pPlayerInfo->iLevelExp;
		int iMaxExp = m_pPlayerInfo->iMaxExp;

		const TEXINFO* pTexture
			= CTextureMgr::Get_Instance()->GetTexture(L"ExpBar");

		if (nullptr == pTexture)
			return;

		D3DXMatrixTranslation(&matTrans, m_vExpBarPos.x, m_vExpBarPos.y, 0.f);

		float fScale = (float)iExp / iMaxExp;
		D3DXMatrixScaling(&matScale, fScale, 1.f, 0.f);

		matWorld = matScale * matTrans;

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);

		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));


		// LevelUp Button
		if (iExp <= iLevelExp)
		{
			pTexture = CTextureMgr::Get_Instance()->GetTexture(L"Button", L"MaxButton", 0);
		}
		else
		{
			pTexture = CTextureMgr::Get_Instance()->GetTexture(L"Button", L"MaxButton", 1);
		}

		if (nullptr == pTexture)
			return;

		D3DXMatrixTranslation(&matTrans, m_vMaxBtnPos.x, m_vMaxBtnPos.y, 0.f);

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);

		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		
		// 스텟 올리기
		for (int i = 0; i < 7; ++i)
		{
			if (m_iPoint[i] <= m_pPlayerInfo->iPoint)
			{
				pTexture = CTextureMgr::Get_Instance()->GetTexture(L"+Button");

				if (nullptr == pTexture)
					return;

				D3DXMatrixTranslation(&matTrans, m_vPlus[i].x, m_vPlus[i].y, 0.f);

				CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);

				float fCX = pTexture->tImgInfo.Width;
				float fCY = pTexture->tImgInfo.Height;

				CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
					NULL,
					&D3DXVECTOR3(fCX*0.5f, fCY*0.f, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}
}

void CStatusWnd::Release(void)
{
	Safe_Delete(m_pObserver);
}
