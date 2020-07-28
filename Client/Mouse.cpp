#include "stdafx.h"
#include "Mouse.h"
#include "ObjMgr.h"
#include "SceneMgr.h"

bool CMouse::m_bIsOnLogoBtn = false;

CMouse::CMouse()
	: m_ePickMode(PM_DEFAULT)
	, m_bIsOn{ 0, }
{
	m_eRenderID = RDR_MOUSE;
}

CMouse::~CMouse()
{
	Release();
}

HRESULT CMouse::Initialize(void)
{
	SCENEID eScene = CSceneMgr::Get_Instance()->Get_SceneID();

	if (eScene == SC_MENU)
	{
		m_eNextState = S_DELAY;
		m_tFrame.wstrObjKey = L"Mouse";
		m_tFrame.wstrStateKey = L"Delay";
	}
	else
	{
		m_eNextState = S_DEFAULT;
		m_tFrame.wstrObjKey = L"Mouse";
		m_tFrame.wstrStateKey = L"Default";
	}

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, 0);

	if (nullptr == pTexInfo)
		return E_FAIL;

	SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

	m_tFrame.iSpriteCnt = 0;
	m_tFrame.fFrameTime = 0;
	m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
	m_tFrame.iSpriteMax = tSprite_Info.iCount;

	ShowCursor(FALSE);
	return S_OK;
}

int CMouse::Progress(void)
{
	SCENEID eScene = CSceneMgr::Get_Instance()->Get_SceneID();

	if (eScene == SC_MENU)
	{
		Move();
		Update_World_Matrix();
		Key_Check();
		Is_OnBtn_InLogo();
		Update_State();
		Move_Frame();
	}
	else
	{
		Move();
		OnEffect();
		Update_World_Matrix();
		Key_Check();
		Update_State();
		Move_Frame();
	}

	return OBJ_NOEVENT;
}

void CMouse::Render(void)
{
	if (m_ePickMode == PM_DEFAULT)
	{
		const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
			m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, m_tFrame.iSpriteCnt
		);
	
		if (nullptr == pTexture)
			return;

		float fCX = float(pTexture->tImgInfo.Width);
		float fCY = float(pTexture->tImgInfo.Height);


		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);


		CGraphicDev::Get_Instance()->GetSprite()->Draw(
			pTexture->pTexture
			, NULL
			, NULL
			, NULL
			, D3DCOLOR_ARGB(255, 255, 255, 255) // 모든 색상을 출력
		);

		//if (m_eCurState == S_DEFAULT || m_eCurState == S_DEFAULT_CLICK)
		//{
		//	CGraphicDev::Get_Instance()->GetSprite()->Draw(
		//		pTexture->pTexture
		//		, NULL
		//		, NULL
		//		, NULL
		//		, D3DCOLOR_ARGB(255, 255, 255, 255) // 모든 색상을 출력
		//	);
		//}
		//else
		//{
		//	CGraphicDev::Get_Instance()->GetSprite()->Draw(
		//		pTexture->pTexture
		//		, NULL
		//		, &D3DXVECTOR3(fCX, fCY, 0.f)
		//		, NULL
		//		, D3DCOLOR_ARGB(255, 255, 255, 255) // 모든 색상을 출력
		//	);
		//}
		
	}

	else if (m_ePickMode == PM_ITEM)
	{
		const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
			m_pPickedItem->Get_ItemInfo().wstrObjKey
		);

		if (nullptr == pTexture)
			return;

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

		float fCX = float(pTexture->tImgInfo.Width);
		float fCY = float(pTexture->tImgInfo.Height);

		CGraphicDev::Get_Instance()->GetSprite()->Draw(
			pTexture->pTexture
			, NULL
			, &D3DXVECTOR3(fCX*0.5f, fCY*0.5f, 0.f)
			, NULL
			, D3DCOLOR_ARGB(180, 255, 255, 255) // 모든 색상을 출력
		);
	}
}

void CMouse::Release(void)
{
}

void CMouse::Move_Frame(void)
{
	m_tFrame.fFrameTime += GET_TIME;

	if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fFrameTime = 0;

		if (
			(m_eCurState != S_DEFAULT_CLICK) &&
			(m_eCurState != S_ATTACK_CLICK) &&
			(m_eCurState != S_CATCH_CLICK) &&
			(m_eCurState != S_DELAY_CLICK)
		)
		{
			m_tFrame.iSpriteCnt++;
		}
	}

	if (m_tFrame.iSpriteCnt >= m_tFrame.iSpriteMax)
	{
		m_tFrame.iSpriteCnt = 0;
	}
}

void CMouse::Late_Progress(void)
{
}

void CMouse::Key_Check()
{
	// 아이템 바닥에 떨구기
	if (m_ePickMode == PM_ITEM)
	{
		if (m_bIsOn[ON_UI] == false) // 유아이가 아니라면 맵이고 거기에 버리기
		{
			if (CKeyMgr::Get_Instance()->KeyDown(VK_LBUTTON, CKeyMgr::S_DEFAULT))
			{
				m_ePickMode = PM_DEFAULT;
				if (m_pPickedItem->Get_ItemInfo().eItemID != IT_SKILL)
				{
					CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
					vector<CItem*>* pInven = pPlayer->Get_InvenPtr();

					// 인벤에서 없애기
					for (auto iter = pInven->begin(); iter != pInven->end(); )
					{
						if (m_pPickedItem == *iter)
						{
							*iter = nullptr;
							iter = pInven->erase(iter);
							// 확인
							CSoundMgr::Get_Instance()->PlaySound(L"Drag.wav", CSoundMgr::CHANNELID::UI);
						}
						else
						{
							++iter;
						}
					}

					CBack* pBack = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background());
					int iIndex = pBack->Get_Tile_Index(GetMouse(), true);

					m_pPickedItem->Set_Pos(iIndex);
					pBack->Push_Item(m_pPickedItem);
				}
			}
		}

		else if (CKeyMgr::Get_Instance()->KeyDown(VK_RBUTTON, CKeyMgr::S_DEFAULT))
		{
			End_Picking();
		}

	}

	// 스킬 선택 시 우 클릭하면 해제
	if (m_eCurState == S_MAGIC)
	{
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_RBUTTON, CKeyMgr::S_DEFAULT))
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
			pPlayer->Cancel_Skill();
		}
	}


	if (m_eCurState == S_MAGIC)
	{
		if (CKeyMgr::Get_Instance()->KeyDown(VK_LBUTTON, CKeyMgr::S_DEFAULT))
		{
			CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
			pPlayer->Use_Skill();
		}
	}

	// Default Click
	if (CKeyMgr::Get_Instance()->KeyPressing(VK_LBUTTON)
		&& m_eCurState == S_DEFAULT)
	{
		m_eNextState = CMouse::S_DEFAULT_CLICK;
		m_tFrame.iSpriteCnt = 0;
	}

	if (m_eCurState == S_DEFAULT_CLICK)
	{
		if (CKeyMgr::Get_Instance()->KeyPressed(VK_LBUTTON, CKeyMgr::S_DEFAULT))
		{
			m_eNextState = S_DEFAULT_CLICK;
			m_tFrame.iSpriteCnt = 0;
		}
	}

	// X Click
	if (CKeyMgr::Get_Instance()->KeyPressing(VK_LBUTTON)
		&& m_eCurState == S_X)
	{
		m_eNextState = CMouse::S_X_CLICK;
		m_tFrame.iSpriteCnt = 0;
	}

	if (m_eCurState == S_X_CLICK)
	{
		if (CKeyMgr::Get_Instance()->KeyPressed(VK_LBUTTON, CKeyMgr::S_DEFAULT))
		{
			m_eNextState = S_X;
			m_tFrame.iSpriteCnt = 0;
		}
	}

	// Attack Click
	if (CKeyMgr::Get_Instance()->KeyPressing(VK_LBUTTON)
		&& m_eCurState == S_ATTACK)
	{
		m_eNextState = CMouse::S_ATTACK_CLICK;
		m_tFrame.iSpriteCnt = 0;
	}
	if (m_eCurState == S_ATTACK_CLICK)
	{
		if (CKeyMgr::Get_Instance()->KeyPressed(VK_LBUTTON, CKeyMgr::S_DEFAULT))
		{
			m_eNextState = S_ATTACK;
			m_tFrame.iSpriteCnt = 0;
		}
	}


	// Catch Click
	if (CKeyMgr::Get_Instance()->KeyPressing(VK_LBUTTON)
		&& m_eCurState == S_CATCH)
	{
		m_eNextState = CMouse::S_CATCH_CLICK;
		m_tFrame.iSpriteCnt = 0;
	}

	if (m_eCurState == S_CATCH_CLICK)
	{
		if (CKeyMgr::Get_Instance()->KeyPressed(VK_LBUTTON, CKeyMgr::S_DEFAULT))
		{
			m_eNextState = S_CATCH;
			m_tFrame.iSpriteCnt = 0;
		}
	}

	// Delay Click
	if (CKeyMgr::Get_Instance()->KeyPressing(VK_LBUTTON)
		&& m_eCurState == S_DELAY)
	{
		m_eNextState = CMouse::S_DELAY_CLICK;
		m_tFrame.iSpriteCnt = 0;
	}
	if (m_eCurState == S_DELAY_CLICK)
	{
		if (CKeyMgr::Get_Instance()->KeyPressed(VK_LBUTTON, CKeyMgr::S_DEFAULT))
		{
			m_eNextState = S_DELAY;
			m_tFrame.iSpriteCnt = 0;
		}
	}

	//// Default Click
	//if (CKeyMgr::Get_Instance()->KeyPressing(VK_LBUTTON)
	//	&& m_eCurState == S_DEFAULT)
	//{
	//	m_eNextState = CMouse::S_DEFAULT_CLICK;
	//	m_tFrame.iSpriteCnt = 0;
	//}

	//if (CKeyMgr::Get_Instance()->KeyPressed(VK_LBUTTON, CKeyMgr::S_MOUSE)
	//	&& m_eCurState == S_DEFAULT_CLICK)
	//{
	//	m_eNextState = S_DEFAULT;
	//	m_tFrame.iSpriteCnt = 0;
	//}

	//// X Click
	//if (CKeyMgr::Get_Instance()->KeyPressing(VK_LBUTTON)
	//	&& m_eCurState == S_X)
	//{
	//	m_eNextState = CMouse::S_X_CLICK;
	//	m_tFrame.iSpriteCnt = 0;
	//}

	//if (CKeyMgr::Get_Instance()->KeyPressed(VK_LBUTTON, CKeyMgr::S_MOUSE)
	//	&& m_eCurState == S_X_CLICK)
	//{
	//	m_eNextState = S_X;
	//	m_tFrame.iSpriteCnt = 0;
	//}

	//// Attack Click
	//if (CKeyMgr::Get_Instance()->KeyPressing(VK_LBUTTON)
	//	&& m_eCurState == S_ATTACK)
	//{
	//	m_eNextState = CMouse::S_ATTACK_CLICK;
	//	m_tFrame.iSpriteCnt = 0;
	//}

	//if (CKeyMgr::Get_Instance()->KeyPressed(VK_LBUTTON, CKeyMgr::S_MOUSE)
	//	&& m_eCurState == S_ATTACK_CLICK)
	//{
	//	m_eNextState = S_ATTACK;
	//	m_tFrame.iSpriteCnt = 0;
	//}

	//// Catch Click
	//if (CKeyMgr::Get_Instance()->KeyPressing(VK_LBUTTON)
	//	&& m_eCurState == S_CATCH)
	//{
	//	m_eNextState = CMouse::S_CATCH_CLICK;
	//	m_tFrame.iSpriteCnt = 0;
	//}

	//if (CKeyMgr::Get_Instance()->KeyPressed(VK_LBUTTON, CKeyMgr::S_MOUSE)
	//	&& m_eCurState == S_CATCH_CLICK)
	//{
	//	m_eNextState = S_CATCH;
	//	m_tFrame.iSpriteCnt = 0;
	//}

	//// Delay Click
	//if (CKeyMgr::Get_Instance()->KeyPressing(VK_LBUTTON)
	//	&& m_eCurState == S_DELAY)
	//{
	//	m_eNextState = CMouse::S_DELAY_CLICK;
	//	m_tFrame.iSpriteCnt = 0;
	//}

	//if (CKeyMgr::Get_Instance()->KeyPressed(VK_LBUTTON, CKeyMgr::S_MOUSE)
	//	&& m_eCurState == S_DELAY_CLICK)
	//{
	//	m_eNextState = S_DELAY;
	//	m_tFrame.iSpriteCnt = 0;
	//}
}

void CMouse::Move()
{
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(g_hWnd, &mouse);

	D3DXVECTOR3 vMouse = { (float)mouse.x, (float)mouse.y, 0.f };
	
	m_tInfo.vPos = vMouse;
}


void CMouse::Back_Default()
{
	if (m_eCurState == CMouse::S_DEFAULT_CLICK)
	{
		if (m_tFrame.iSpriteCnt >= m_tFrame.iSpriteMax - 1)
		{
			m_eNextState = CMouse::S_DEFAULT;
		}
	}

	//if (m_eCurState == CMouse::S_DELAY_CLICK)
	//{
	//	if (m_tFrame.iSpriteCnt >= m_tFrame.iSpriteMax - 1)
	//	{
	//		m_eNextState = CMouse::S_DELAY;
	//	}
	//}
}

void CMouse::Is_OnBtn_InLogo(void)
{
	if (!CTextureMgr::Get_Instance()->Is_Load_Completed())
	{
		if (true == m_bIsOnLogoBtn)
		{
			m_eNextState = S_DEFAULT;
		}
		else
		{
			m_eNextState = S_DELAY;
		}

		m_bIsOnLogoBtn = false;
	}
}

void CMouse::Cancel_Skill(void)
{
	m_bIsOn[ON_SKILL] = false;
}

void CMouse::Reset_On_Array(void)
{
	for (int i = 0; i < ON_END; ++i)
	{
		m_bIsOn[i] = false;
	}
}

void CMouse::Pick(CItem * pItem)
{
	m_ePickMode = PM_ITEM;
	m_pPickedItem = pItem;	
	CSoundMgr::Get_Instance()->PlaySound(L"BtnClick", CSoundMgr::CHANNELID::UI);
}

void CMouse::End_Picking(void)
{
	m_ePickMode = PM_DEFAULT;
	m_pPickedItem = nullptr;
}

void CMouse::Update_World_Matrix(void)
{
	D3DXMATRIX matTrans, matScale, matRotZ;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans
		, m_tInfo.vPos.x
		, m_tInfo.vPos.y
		, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

}

void CMouse::OnEffect(void)
{
	// UI > NPC, Monster > Item > Tile
	if (m_bIsOn[ON_UI])
	{
		m_eNextState = S_DEFAULT;
	}
	
	else if (m_bIsOn[ON_SKILL])
	{
		m_eNextState = S_MAGIC;
	}

	else if (m_bIsOn[ON_MONSTER])
	{
		m_eNextState = S_ATTACK;
	}

	else if (m_bIsOn[ON_NPC])
	{
		m_eNextState = S_CHAT;
	}

	else if (m_bIsOn[ON_ITEM])
	{
		m_eNextState = S_CATCH;
	}

	//if (m_ePickMode == PM_DEFAULT)
	else
	{
		m_iTile_Idx = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background())->Get_Tile_Index(m_tInfo.vPos, true);
		static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background())->Get_Option_Draw(m_tInfo.vPos, m_eDraw, m_eOption, true);

		switch (m_eDraw)
		{
		case TD_NORMAL:
			m_eNextState = S_DEFAULT;
			break;

		case TD_NO_MOVE:
			m_eNextState = S_X;
			break;

		case TD_TRANSPARENT:
			m_eNextState = S_DEFAULT;
			break;

		case TD_GATE:
			m_eNextState = S_GATE;
			break;
		}
	}
}

void CMouse::Update_State(void)
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case S_DEFAULT:
			m_tFrame.wstrStateKey = L"Default";
			break;

		case S_DEFAULT_CLICK:
			m_tFrame.wstrStateKey = L"DefaultClick";
			break;

		case S_X:
			m_tFrame.wstrStateKey = L"X";
			break;

		case S_X_CLICK:
			m_tFrame.wstrStateKey = L"XClick";
			break;

		case S_ATTACK:
			m_tFrame.wstrStateKey = L"Attack";
			break;

		case S_ATTACK_CLICK:
			m_tFrame.wstrStateKey = L"AttackClick";
			break;

		case S_CATCH:
			m_tFrame.wstrStateKey = L"Catch";
			break;

		case S_CATCH_CLICK:
			m_tFrame.wstrStateKey = L"CatchClick";
			break;

		case S_DELAY:
			m_tFrame.wstrStateKey = L"Delay";
			break;

		case S_DELAY_CLICK:
			m_tFrame.wstrStateKey = L"DelayClick";
			break;

		case S_CHAT:
			m_tFrame.wstrStateKey = L"Chat";
			break;

		case S_GATE:
			m_tFrame.wstrStateKey = L"Gate";
			break;

		case S_MAGIC:
			m_tFrame.wstrStateKey = L"Magic";
			break;
		}

		m_eCurState = m_eNextState;

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
			m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, 0);

		if (nullptr == pTexInfo)
			return;

		SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

		m_tFrame.iSpriteCnt = 0;
		m_tFrame.fFrameTime = 0;
		m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
		m_tFrame.iSpriteMax = tSprite_Info.iCount;
	}

}
