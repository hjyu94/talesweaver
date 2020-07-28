#include "stdafx.h"
#include "Chat.h"
#include "KeyMgr2.h"
#include "UIObserver.h"
#include "DataSubject.h"
#include "ObjMgr.h"
CChat::CChat()
{
	m_bIsEditing = false;
	m_bCapsLock = false;
}


CChat::~CChat()
{
}

void CChat::Update_World_Matrix(void)
{
}

void CChat::Update_State(void)
{
}

HRESULT CChat::Initialize(void)
{
	m_tInfo.vPos = D3DXVECTOR3(200.f, WINCY-101.f, 0.f);
	m_tFrame.wstrObjKey = L"ChatWnd";

	// 키 설정
	char arrUpperCase[] = {
		L'A',
		L'B',
		L'C',
		L'D',
		L'E',
		L'F',
		L'G',
		L'H',
		L'I',
		L'J',
		L'K',
		L'L',
		L'M',
		L'N',
		L'O',
		L'P',
		L'Q',
		L'R',
		L'S',
		L'T',
		L'U',
		L'V',
		L'W',
		L'X',
		L'Y',
		L'Z'
	};
	char arrLowerCase[] = {
		L'a',
		L'b',
		L'c',
		L'd',
		L'e',
		L'f',
		L'g',
		L'h',
		L'i',
		L'j',
		L'k',
		L'l',
		L'm',
		L'n',
		L'o',
		L'p',
		L'q',
		L'r',
		L's',
		L't',
		L'u',
		L'v',
		L'w',
		L'x',
		L'y',
		L'z'
	};

	for (int i = 0; i < 26; ++i)
	{
		m_arrUpperCase[i] = arrUpperCase[i];
		m_arrLowerCase[i] = arrLowerCase[i];
	}


	m_pObserver = new CUIObserver;
	CDataSubject::Get_Instance()->Subscribe(m_pObserver);

	return S_OK;
}

int CChat::Progress(void)
{
	m_fTime += GET_TIME;

	// rect
	m_InputPos = m_tInfo.vPos + D3DXVECTOR3{ 138.f + 5.f, 75.f + 1.f, 0.f }; // LT
	m_InputRect = Calculate_Rect(m_InputPos, 280.f, 15.f, false);

	for (int i = 0; i < 4; ++i)
	{
		m_ChatPos[i] = m_tInfo.vPos + D3DXVECTOR3{ 10.f, 9.f + 15.f * i, 0.f };
		m_ChatRect[i] = Calculate_Rect(m_ChatPos[i], 490.f, 15.f, false); // LT
	}

	// Scroll
	m_ScrollPos[SCR_BACK] = D3DXVECTOR3{ 511.f, 15.f, 0.f } +m_tInfo.vPos; // LT
	m_ScrollRect[SCR_BACK] = Calculate_Rect(m_ScrollPos[SCR_BACK], 6.f, 45.f, false);

	m_ScrollPos[SCR_UP] = D3DXVECTOR3{ 514.f, 16.f, 0.f } +m_tInfo.vPos; // CENTER
	m_ScrollPos[SCR_DOWN] = D3DXVECTOR3{ 514.f, 61.f, 0.f } +m_tInfo.vPos; // CENTER
	m_ScrollRect[SCR_UP] = Calculate_Rect(m_ScrollPos[SCR_UP], 12.f, 13.f);
	m_ScrollRect[SCR_DOWN] = Calculate_Rect(m_ScrollPos[SCR_DOWN], 12.f, 13.f);

	// 입력
	// 확인. VK_RETURN
	if (CKeyMgr2::Get_Instance()->KeyDown(VK_RETURN))
	{
		// 입력한 텍스트 추가하기
		if (m_bIsEditing)
		{
			if (m_wstrText != L"")
			{
				m_ChatList.push_back(L"Rick : " + m_wstrText);
				CSoundMgr::Get_Instance()->PlaySound(L"BtnClick.wav", CSoundMgr::UI);
				m_bTextPush = true;
			}
				
			m_wstrText = L"";

			m_fTime = 0.f;

			// 밑으로 당기기
			if (m_ChatList.size() > 4)
			{
				m_iStartIndex = m_ChatList.size() - 4;
			}
		}

		m_bIsEditing = !m_bIsEditing;
	}

	if (CKeyMgr2::Get_Instance()->KeyDown(VK_CAPITAL))
	{
		m_bCapsLock = !m_bCapsLock;
	}

	if (m_bIsEditing)
	{
		// 알파벳 입력
		for (int i = 0; i < 26; ++i)
		{
			if (CKeyMgr2::Get_Instance()->KeyDown(m_arrUpperCase[i]))
			{
				if (m_bCapsLock)
					m_wstrText += m_arrUpperCase[i];
				else
					m_wstrText += m_arrLowerCase[i];
			}
		}

		if (CKeyMgr2::Get_Instance()->KeyDown(VK_BACK))
		{
			m_wstrText = m_wstrText.substr(0, m_wstrText.length() - 1);
		}

		if (CKeyMgr2::Get_Instance()->KeyDown(VK_SPACE))
		{
			m_wstrText += L" ";
		}
	}

	CKeyMgr2::Get_Instance()->KeyUpdate();

	// 스크롤 위 아래 버튼 클릭 시
	POINT pt = GetMousePt();

	if (PtInRect(&m_ScrollRect[SCR_UP], pt))
	{
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_SHOP_WND))
		{
			m_iStartIndex--;
			if (m_iStartIndex < 0) m_iStartIndex = 0;
			CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::UI);
		}
	}

	if (PtInRect(&m_ScrollRect[SCR_DOWN], pt))
	{
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_SHOP_WND))
		{
			if (int(m_ChatList.size()) - 4 > m_iStartIndex)
			{
				m_iStartIndex++;
			}
			CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::UI);
		}
	}

	if (PtInRect(&m_tRect, pt))
	{
		static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse())->Set_State(CMouse::ON_UI, true);
	}

	return 0;
}

void CChat::Render(void)
{
	D3DXMATRIX	 matTrans, matScale, matParent, matWorld;

	// UI
	{
		const TEXINFO* pTexture
			= CTextureMgr::Get_Instance()->GetTexture(m_tFrame.wstrObjKey);

		if (nullptr == pTexture)
			return;

		float fX = float(pTexture->tImgInfo.Width) ;
		float fY = float(pTexture->tImgInfo.Height);

		m_tRect = Calculate_Rect(m_tInfo.vPos, fX, fY, false);
		CGraphicDev::Get_Instance()->Draw_Rect(m_tRect);

		D3DXMATRIX	matTrans;
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// 입력 중인 경우
	if (m_bIsEditing)
	{
		const TEXINFO* pTexture
			= CTextureMgr::Get_Instance()->GetTexture(L"ChatEdit");

		if (nullptr == pTexture)
			return;

		float fX = float(pTexture->tImgInfo.Width) * 0.5f;
		float fY = float(pTexture->tImgInfo.Height) * 0.5f;

		D3DXMATRIX	matTrans;
		D3DXMatrixTranslation(&matTrans, m_InputPos.x, m_InputPos.y, 0.f);

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(50, 255, 255, 255));
	}

	// 스크롤 출력
	// Up
	{
		const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(L"UpScroll");

		if (nullptr == pTexture)
			return;

		float fX = float(pTexture->tImgInfo.Width) * 0.5f;
		float fY = float(pTexture->tImgInfo.Height) * 0.5f;

		D3DXMATRIX	matTrans;
		D3DXMatrixTranslation(&matTrans, m_ScrollPos[SCR_UP].x, m_ScrollPos[SCR_UP].y, 0.f);

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
		D3DXMatrixTranslation(&matTrans, m_ScrollPos[SCR_DOWN].x, m_ScrollPos[SCR_DOWN].y, 0.f);

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fX, fY, 0.f),
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// 플레이어 말풍선 출력
	if (m_fTime <= 3.f && !m_ChatList.empty())
	{
   		CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
		D3DXVECTOR3	vPlayer = pPlayer->Get_Info().vPos;

		m_vTextCenter = vPlayer - m_vScroll - D3DXVECTOR3{ 0.f, 70.f, 0.f };
		m_rcText = Calculate_Rect(m_vTextCenter, 170.f, 30.f);
		
		const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(L"TextMessage");

		if (nullptr == pTexture)
			return;

		float fCX = float(pTexture->tImgInfo.Width) * 0.5f;
		float fCY = float(pTexture->tImgInfo.Height) * 0.5f;

		D3DXMatrixTranslation(&matTrans, m_vTextCenter.x, m_vTextCenter.y, 0.f);
		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fCX, fCY, 0.f),
			NULL,
			D3DCOLOR_ARGB(200, 255, 255, 255));

		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
			CGraphicDev::Get_Instance()->GetSprite(),
			m_ChatList.back().c_str(),
			lstrlen(m_ChatList.back().c_str()),
			&m_rcText,
			DT_CENTER | DT_VCENTER,
			D3DCOLOR_ARGB(255, 255, 255, 255));
		
		CGraphicDev::Get_Instance()->Draw_Rect(m_rcText);

	}

	if (m_fTime > 0.5f)
	{
		m_bTextPush = false;
	}

	// 입력중인 텍스트 출력
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);

	CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
		CGraphicDev::Get_Instance()->GetSprite(),
		m_wstrText.c_str(),
		lstrlen(m_wstrText.c_str()),
		&m_InputRect,
		DT_LEFT,
		D3DCOLOR_ARGB(255, 255, 255, 255));


	// 채팅 내용 출력
	auto iter = m_ChatList.begin();
	std::advance(iter, m_iStartIndex);

	for (int i = 0; i < 4; ++i)
	{
		if (i >= m_ChatList.size())
			break;

		CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
			CGraphicDev::Get_Instance()->GetSprite(),
			(*iter).c_str(),
			lstrlen((*iter).c_str()),
			&m_ChatRect[i],
			DT_LEFT,
			D3DCOLOR_ARGB(255, 255, 255, 255));

		iter++;
	}

	// Rect 확인 디버깅
	CGraphicDev::Get_Instance()->Draw_Rect(m_InputRect);
	CGraphicDev::Get_Instance()->Draw_Rect(m_ScrollRect[SCR_UP]);
	CGraphicDev::Get_Instance()->Draw_Rect(m_ScrollRect[SCR_DOWN]);
	CGraphicDev::Get_Instance()->Draw_Rect(m_ScrollRect[SCR_BACK]);
	for (int i = 0; i < 4; ++i)
	{
		CGraphicDev::Get_Instance()->Draw_Rect(m_ChatRect[i]);
	}
}

void CChat::Release(void)
{
	m_ChatList.clear();
}

