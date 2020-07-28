#include "stdafx.h"
#include "Player.h"
#include "ObjMgr.h"
#include "Back.h"
#include "DataSubject.h"
#include "ObjFactory.h"
#include "Damage.h"
#include "SceneMgr.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	m_tFrame.wstrObjKey = L"Lucian";
	m_tFrame.wstrStateKey = L"Stand_";
	m_wstrDir = L"D";
	m_tInfo.vDir = { 0.f, 1.f, 0.f };
	m_eMove = MS_WALK;
	m_eCurState = S_STAND;
	m_eCurDir = D_DOWN;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey+ m_wstrDir, 0);

	if (nullptr == pTexInfo)
		return E_FAIL;

	SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

	m_tFrame.iSpriteCnt = 0;
	m_tFrame.fFrameTime = 0;
	m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
	m_tFrame.iSpriteMax = tSprite_Info.iCount;

	m_vMoveDest = {0.f, 0.f, 0.f};
	m_fSpeed = 300.f;

	// INFO
	m_tPlayerInfo = PLAYER_INFO{
		1, 30, 30, 8, 10, 15, 15, 20,
		100, 100, 0, 100, 150, 100000, 1, L"Rick" };

	m_tStatus = STATUS_INFO{ 100, 100, 100, 100 };

	
	// 인벤토리
	m_vecInven.reserve(12);

	CPotion* pPotion = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP30", 3));
	m_vecInven.push_back(pPotion);
	
	pPotion = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"MP30", 3));
	m_vecInven.push_back(pPotion);

	// 장비
	for (int i = 0; i < EQ_END; ++i)
	{
		m_arrEquip[i] = nullptr;
	}

	// 무기
	m_pWeapon = new CWeapon(L"RedSword");
	m_pWeapon->Initialize();
	m_pWeapon->Set_Show(false);
	m_pWeapon->Set_Owner(this);

	// 데미지
	m_bShield = false;

	// On
	Reset_On_Array();

	CDataSubject::Get_Instance()->AddData(PLAYER_INVENTORY, &m_vecInven);
	CDataSubject::Get_Instance()->AddData(STATUS, &m_tStatus);
	CDataSubject::Get_Instance()->AddData(PLAYER_STATUS, &m_tPlayerInfo);
	CDataSubject::Get_Instance()->AddData(PLAYER_MATRIX, &m_matTrans);
	CDataSubject::Get_Instance()->AddData(PLAYER_DIR, &m_eCurDir);
	CDataSubject::Get_Instance()->AddData(PLAYER_MOVESTATE, &m_eMove);
	
	return S_OK;
}

int CPlayer::Progress(void)
{
	Key_Check();
	Move();
	Update_Direction();
	Is_On_Gate(); 
	OnEffect();
	Update_State();
	Offset();
	Update_World_Matrix();
	Move_Frame();
	IsRecovering();

	// 위치 넘기기
	CDataSubject::Get_Instance()->Notify(PLAYER_MATRIX, &m_matTrans);

	// 무기
	m_pWeapon->Progress();

	if (m_arrEquip[IT_ARMS] != nullptr
		&& (m_eCurState == S_STAND || m_eCurState == S_HORCUT || m_eCurState == S_STAB || m_eCurState == S_VERCUT)
		)
	{
		m_pWeapon->Set_Show(true);
	}
	else
	{
		m_pWeapon->Set_Show(false);
	}
	
	// 디버깅
	if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000)
	{
		m_tStatus.iHp = 10;
		CDataSubject::Get_Instance()->Notify(STATUS, &m_tStatus);
	}
	if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000)
	{
		m_tStatus.iMp = 10;
		CDataSubject::Get_Instance()->Notify(STATUS, &m_tStatus);
	}
	if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
	{
		m_tPlayerInfo.iSp = 10;
		CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);
	}
	if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000)
	{
		m_tStatus.iHp = m_tStatus.iMaxHp;
		m_tStatus.iMp = m_tStatus.iMaxMp;
		m_tPlayerInfo.iSp = m_tPlayerInfo.iMaxSp;
		CDataSubject::Get_Instance()->Notify(STATUS, &m_tStatus);
		CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);
	}
	if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
	{
		Level_Up(true);
		m_tPlayerInfo.iExp = 0;
	}
	if(CKeyMgr::Get_Instance()->KeyDown(KEY_8, CKeyMgr::S_PLAYER))
	{
		m_bPowerful = !m_bPowerful;
	}
	if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000)
	{
		m_tPlayerInfo.iHack = 3000;
		CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);
	}

	return 0;
}

void CPlayer::Render(void)
{
	const TEXINFO* pTexture = nullptr;

	if (!m_bIsDead)
	{
		pTexture = CTextureMgr::Get_Instance()->GetTexture(
			m_tFrame.wstrObjKey, m_tFrame.wstrStateKey + m_wstrDir, m_tFrame.iSpriteCnt
		);
	}
	else
	{
		pTexture = CTextureMgr::Get_Instance()->GetTexture(
			m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, m_tFrame.iSpriteCnt
		);

	}

	float fCX = float(pTexture->tImgInfo.Width) * 0.5f;
	float fCY = float(pTexture->tImgInfo.Height) * 0.5f;

	vec vPos = m_tInfo.vPos - m_vScroll;
	m_tRect = Calculate_Rect(vPos, float(pTexture->tImgInfo.Width), float(pTexture->tImgInfo.Height));

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	if (m_bIsOn[ON_OBJECT]) // 오브젝트와 부딪히면 
	{
		CGraphicDev::Get_Instance()->GetSprite()->Draw(
			pTexture->pTexture
			, NULL
			, &D3DXVECTOR3(fCX, fCY, 0.f)
			, NULL
			, D3DCOLOR_ARGB(150, 255, 255, 255) // 모든 색상을 출력
		);
	}
	else // 오브젝트와 부딪히지 않음 -> 정상 출력
	{
		CGraphicDev::Get_Instance()->GetSprite()->Draw(
			pTexture->pTexture
			, NULL
			, &D3DXVECTOR3(fCX, fCY, 0.f)
			, NULL
			, D3DCOLOR_ARGB(255, 255, 255, 255) // 모든 색상을 출력
		);
	}

	// 무기
	if(!m_bIsDead)
		m_pWeapon->Render();


	// 디버깅
	CGraphicDev::Get_Instance()->Draw_Rect(m_tRect);
}

void CPlayer::Release(void)
{
	//... 아이템 먹으면 터짐?? 언제 터짐??
	for (auto iter = m_vecInven.begin(); iter != m_vecInven.end(); ++iter)
	{
		if (*iter)
		{
			delete *iter;
			*iter = nullptr;
		}
	}
	m_vecInven.clear();

	for (int i = 0; i < EQ_END; ++i)
	{
		if (nullptr != m_arrEquip[i])
		{
			delete m_arrEquip[i];
			m_arrEquip[i] = nullptr;
		}
	}

	if (m_pWeapon)
	{
		delete m_pWeapon;
		m_pWeapon = nullptr;
	}
}

void CPlayer::Move_Frame(void) 
{
	m_tFrame.fFrameTime += GET_TIME;

	if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fFrameTime = 0;
		m_tFrame.iSpriteCnt++;
	}

	if (m_tFrame.iSpriteCnt >= m_tFrame.iSpriteMax)
	{
		if (m_eCurState == S_HORCUT)
			m_bIsOn[ON_HORCUT] = false;

		if (m_eCurState == S_STAB)
			m_bIsOn[ON_STAB] = false;

		if (m_eCurState == S_VERCUT)
			m_bIsOn[ON_VERCUT] = false;

		m_tFrame.iSpriteCnt = 0;
	}
}


void CPlayer::Update_Direction()
{
	if (m_tInfo.vDir.x > 0)
	{
		if (m_tInfo.vDir.y > 0)
		{
			m_eNextDir = D_RD;
		}
		else if (m_tInfo.vDir.y < 0)
		{
			m_eNextDir = D_RU;
		}
		else
		{
			m_eNextDir = D_RIGHT;
		}
	}
	else if (m_tInfo.vDir.x < 0)
	{
		if (m_tInfo.vDir.y > 0)
		{
			m_eNextDir = D_LD;
		}
		else if (m_tInfo.vDir.y < 0)
		{
			m_eNextDir = D_LU;
		}
		else
		{
			m_eNextDir = D_LEFT;
		}
	}
	else
	{
		if (m_tInfo.vDir.y >= 0)
		{
			m_eNextDir = D_DOWN;
		}
		else
		{
			m_eNextDir = D_UP;
		}
	}

	CDataSubject::Get_Instance()->Notify(PLAYER_DIR, &m_eCurDir);
}

void CPlayer::Update_World_Matrix()
{
	
	if (!m_bIsDead)
	{
		const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
			m_tFrame.wstrObjKey, m_tFrame.wstrStateKey + m_wstrDir, m_tFrame.iSpriteCnt
		);


		if (pTexture->tToolInfo.bReverse)
			D3DXMatrixScaling(&m_matScale, -1.f, 1.f, 0.f);
		else
			D3DXMatrixScaling(&m_matScale, 1.f, 1.f, 0.f);

		D3DXMatrixTranslation(&m_matTrans
			, m_tInfo.vPos.x - m_vScroll.x
			, m_tInfo.vPos.y - m_vScroll.y
			, 0.f);
	}
	else
	{
		if (m_eCurDir == D_RIGHT || m_eCurDir == D_RU || m_eCurDir == D_RD)
		{
			D3DXMatrixScaling(&m_matScale, -1.f, 1.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&m_matScale, 1.f, 1.f, 0.f);
		}

		D3DXMatrixTranslation(&m_matTrans
			, m_tInfo.vPos.x - m_vScroll.x
			, m_tInfo.vPos.y - m_vScroll.y
			, 0.f);
	}

	m_tInfo.matWorld = m_matScale * m_matTrans;
}

void CPlayer::Key_Check()
{
	// 이동하기
	if (CObjMgr::Get_Instance()->Get_Mouse_State() != CMouse::S_X)
	{
		if (!static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse())->Is_On_UI()
			&& m_wstrSkill == L"")
		{
			if (m_eMove == MS_WALK)
			{
				if (CKeyMgr::Get_Instance()->KeyDown(KEY_RBUTTON, CKeyMgr::S_PLAYER))
				{
					m_bIsOn[ON_WALK] = true;
					m_eNextState = S_WALK;
					m_fSpeed = 200;
					Astar_Start();
				}
			}
			else if (m_eMove == MS_RUN)
			{
				if (CKeyMgr::Get_Instance()->KeyDown(KEY_RBUTTON, CKeyMgr::S_PLAYER))
				{
					m_bIsOn[ON_WALK] = true;
					m_eNextState = S_RUN;
					m_fSpeed = 350;
					Astar_Start();
					m_tPlayerInfo.iSp -= 3.f*GET_TIME; // SP 감소
					
					if (m_tPlayerInfo.iSp <= 0)
						m_eMove = MS_WALK;
					
					CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);
				}
			}
		}
	}

	// 걷기, 달리기 모드 전환
	if (CKeyMgr::Get_Instance()->KeyDown(KEY_R, CKeyMgr::S_PLAYER))
	{
		switch (m_eMove)
		{
		case CPlayer::MS_WALK:
			m_eMove = MS_RUN;
			break;

		case CPlayer::MS_RUN:
			m_eMove = MS_WALK;
			break;
		}

		CDataSubject::Get_Instance()->Notify(PLAYER_MOVESTATE, &m_eMove);
	}

	// 앉기
	if (CKeyMgr::Get_Instance()->KeyDown(KEY_S, CKeyMgr::S_PLAYER))
	{
		CChat* pChat = static_cast<CChat*>(CObjMgr::Get_Instance()->Get_ChatUI());

		if (!pChat->Is_Editing())
		{
			if (m_eCurState == S_SIT)
			{
				m_bIsOn[ON_SIT] = false;
			}
			else
			{
				m_bIsOn[ON_SIT] = true;
				m_vMoveDest = m_tInfo.vPos;
			}
		}
	}

	// 공격하기
	if (m_pTarget != nullptr)
	{
		if (CObjMgr::Get_Instance()->Get_Mouse_State() == CMouse::S_ATTACK
			&& Get_Distance(m_tInfo.vPos, m_pTarget->Get_Info().vPos) < 100.f)
		{
			if (CKeyMgr::Get_Instance()->KeyPressed(KEY_LBUTTON, CKeyMgr::S_PLAYER))
			{
				// 할일 todo 확인 무기에 따라 hack인지 stab인지 나눌 것
				if(m_bNoPower)
					Attack(m_pTarget, 0.f);
				else
					Attack(m_pTarget, m_tPlayerInfo.iHack * 0.3);
			}
		}
	}
}

void CPlayer::Move()
{
	if (m_eCurState != S_SIT && m_eNextState != S_SIT && m_bIsOn[ON_WALK] == true)
	{
		if (m_vMoveDest != D3DXVECTOR3(0.f, 0.f, 0.f))
		{
			// Astar Move
			if (!m_BestList.empty())
			{
				CBack* pBackground = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background());
				vector<TILE*>* vecTile = pBackground->Get_TileVecPtr();

				m_iDst_Idx = m_BestList.front();
				m_iCur_Idx = pBackground->Get_Tile_Index(m_tInfo.vPos);
				
				if (m_iCur_Idx == m_iDst_Idx)
					m_BestList.pop_front();

				m_tInfo.vDir = (*vecTile)[m_iDst_Idx]->vPos - m_tInfo.vPos;
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
				m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * GET_TIME;

			}

			// 마우스 피킹까지 정밀하게 더 이동
			else if (Get_Distance(m_tInfo.vPos, m_vMoveDest) > 5.f)
			{
				m_tInfo.vDir = m_vMoveDest - m_tInfo.vPos;
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
				m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * GET_TIME;

			}

			else
			{
				// 이동이 모두 끝났을 때, 정지한 스프라이트 출력
				m_eNextState = S_STAND;
				m_bIsOn[ON_WALK] = false;
			}
		}
	}
}

void CPlayer::Move(D3DXVECTOR3 vDestination)
{
	m_vMoveDest = vDestination;
}

void CPlayer::Is_On_Gate()
{
	TILE_DRAW	eDraw;
	TILE_OPTION eOption;

	CBack* pBack = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background());
	pBack->Get_Option_Draw(m_tInfo.vPos, eDraw, eOption);

	if (eDraw == TD_GATE)
	{
		if (CObjMgr::Get_Instance()->Get_Mouse_State() == CMouse::S_GATE)
		{
			if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_PLAYER))
			{
				m_vMoveDest = { 0.f, 0.f, 0.f };
				m_eNextState = S_STAND;
				int iIndex = pBack->Get_Tile_Index(m_tInfo.vPos);

				// 타일에 있는 정보를 읽어와서 그 스테이지로 이동시키기
				vector<TILE*>* pTileVector = pBack->Get_TileVecPtr();
				TILE* pTile = (*pTileVector)[iIndex];
				wstring wstrOldStage = pBack->Get_wstrStage();
				wstring wstrNewStage = pTile->wstrInfo;

				map<wstring, BACK_INFO*>* mapBackInfo = CMapMgr::Get_Instance()->Get_BackInfoPtr();
				auto iter = mapBackInfo->find(wstrOldStage);
				
				if (iter == mapBackInfo->end())
					return;

				iter->second->iPlayerIndex = iIndex;
				m_eNextState = S_STAND;

				if (wstrNewStage == L"Town1")
				{
					CSceneMgr::Get_Instance()->SetScene(SC_TOWN1);
				}
				else if (wstrNewStage == L"Store0")
				{
					CSceneMgr::Get_Instance()->SetScene(SC_STORE0);
				}
				else if (wstrNewStage == L"Store1")
				{
					CSceneMgr::Get_Instance()->SetScene(SC_STORE1);
				}
				else if (wstrNewStage == L"Store2")
				{
					CSceneMgr::Get_Instance()->SetScene(SC_STORE2);
				}
				else if (wstrNewStage == L"Field0")
				{
					CSceneMgr::Get_Instance()->SetScene(SC_FIELD0);
				}
				else if (wstrNewStage == L"Field3")
				{
					CSceneMgr::Get_Instance()->SetScene(SC_FIELD3);
				}

				//// 현재 타일, 오브젝트 반납
				//Release();

				//// 새로운 스테이지의 타일, 오브젝트 저장
				//Load_File();

				//iter = m_mapBackInfo.find(m_tFrame.wstrObjKey);
				//CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
				//pPlayer->Set_Pos(iter->second->iPlayerIndex);
				//pPlayer->Set_Direction(iter->second->ePlayerDir);
			}
		}
	}
}


void CPlayer::OnEffect(void)
{
	m_fDamageTime += GET_TIME;

	if (m_bIsOn[ON_WALK])
	{
		m_eNextState = S_WALK;
	}
	else if (m_bIsOn[ON_STAB])
	{
		m_eNextState = S_STAB;
	}
	else if (m_bIsOn[ON_HORCUT])
	{
		m_eNextState = S_HORCUT;
	}
	else if (m_bIsOn[ON_VERCUT])
	{
		m_eNextState = S_VERCUT;
	}
	else if (m_bIsOn[ON_DAMAGE])
	{
		if (m_fDamageTime <= 1.f)
		{
			m_eNextState = S_DAMAGE;
			m_tInfo.vPos += m_vNutBackDir * GET_TIME * 20.f;

			CBack* pBack = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background());
			TILE_DRAW eDraw;
			TILE_OPTION eOption;
			pBack->Get_Option_Draw(m_tInfo.vPos, eDraw, eOption);

			if (TO_NO_MOVE == eOption)
				m_tInfo.vPos -= m_vNutBackDir * GET_TIME * 220 * 1.5f;

			m_bIsOn[ON_WALK] = false;
		}
		else
			m_bIsOn[ON_DAMAGE] = false;
	}
	else if (m_bIsOn[ON_SIT])
	{
		m_eNextState = S_SIT;
	}
	else
	{
		m_eNextState = S_STAND;
	}
	

}

void CPlayer::Offset()
{
	RECT boundary = {
		LONG(WINCX / 3.f)
		, LONG(WINCY / 3.f)
		, LONG(WINCX / 3.f * 2)
		, LONG(WINCY / 3.f * 2)
	};

	float fPlayerX = m_tInfo.vPos.x - m_vScroll.x;
	float fPlayerY = m_tInfo.vPos.y - m_vScroll.y;

	if (boundary.left > fPlayerX) // 플레이어가 화면 왼쪽에 치우쳐져 있으면
		m_vScroll.x -= m_fSpeed * GET_TIME;
	else if (boundary.right < fPlayerX) // 플레이어가 화면 오른쪽에 치우쳐져 있으면
		m_vScroll.x += m_fSpeed * GET_TIME;

	if (boundary.top > fPlayerY) // 플레이어가 화면 위쪽에 치우쳐져 있으면
		m_vScroll.y -= m_fSpeed * GET_TIME;
	else if (boundary.bottom < fPlayerY) // 플레이어가 화면 아래쪽에 치우쳐져 있으면
		m_vScroll.y += m_fSpeed * GET_TIME;


	//# Scroll Lock

	int iWidth = 0;
	int iHeight = 0;

	CBack* pBackground = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background());
	pBackground->Get_Map_Size(iWidth, iHeight);

	if (m_vScroll.x < 0) m_vScroll.x = 0;
	else if (m_vScroll.x > iWidth - WINCX) m_vScroll.x = float(iWidth - WINCX);

	if (m_vScroll.y < 0) m_vScroll.y = 0;
	else if (m_vScroll.y > iHeight - WINCY) m_vScroll.y = float(iHeight - WINCY);
}

void CPlayer::Update_State()
{
	if (!m_bIsDead)
	{
		if (m_eCurState != m_eNextState)
		{
			switch (m_eNextState)
			{
			case S_STAND:
				m_tFrame.wstrStateKey = L"Stand_";
				break;
			case S_RUN:
				m_tFrame.wstrStateKey = L"Run_";
				break;
			case S_SIT:
				m_tFrame.wstrStateKey = L"Sit_";
				break;
			case S_SKILL:
				m_tFrame.wstrStateKey = L"Skill_D";
				break;
			case S_HORCUT:
				m_tFrame.wstrStateKey = L"HorCut_";
				break;
			case S_STAB:
				m_tFrame.wstrStateKey = L"Stab_";
				break;
			case S_VERCUT:
				m_tFrame.wstrStateKey = L"VerCut_";
				break;
			case S_WALK:
				m_tFrame.wstrStateKey = L"Walk_";
				break;
			case S_DAMAGE:
				m_tFrame.wstrStateKey = L"Damage_";
				break;
			}

			m_eCurState = m_eNextState;

			const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
				m_tFrame.wstrObjKey, m_tFrame.wstrStateKey + m_wstrDir, 0);

			if (nullptr == pTexInfo)
				return;

			SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

			m_tFrame.iSpriteCnt = 0;
			m_tFrame.fFrameTime = 0;
			m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
			m_tFrame.iSpriteMax = tSprite_Info.iCount;
		}
	}

	if (m_eCurDir != m_eNextDir)
	{
		switch (m_eNextDir)
		{
		case D_UP:
			m_wstrDir = L"U";
			break;

		case D_DOWN:
			m_wstrDir = L"D";
			break;

		case D_LEFT:
			m_wstrDir = L"L";
			break;

		case D_RIGHT:
			m_wstrDir = L"R";
			break;

		case D_LU:
			m_wstrDir = L"LU";
			break;

		case D_RU:
			m_wstrDir = L"RU";
			break;

		case D_LD:
			m_wstrDir = L"LD";
			break;

		case D_RD:
			m_wstrDir = L"RD";
			break;
		}

		m_eCurDir = m_eNextDir;

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
			m_tFrame.wstrObjKey, m_tFrame.wstrStateKey + m_wstrDir, 0);

		if (nullptr == pTexInfo)
			return;

		SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

		m_tFrame.iSpriteCnt = 0;
		m_tFrame.fFrameTime = 0;
		m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
		m_tFrame.iSpriteMax = tSprite_Info.iCount;
	}
}

void CPlayer::Back_Default()
{
}

void CPlayer::IsRecovering()
{
	if (m_eCurState == S_SIT)
	{
		m_tPlayerInfo.iSp += 1;
		if (m_tPlayerInfo.iSp >= m_tPlayerInfo.iMaxSp)
			m_tPlayerInfo.iSp = m_tPlayerInfo.iMaxSp;

		m_tStatus.iMp += 1;
		if (m_tStatus.iMp >= m_tPlayerInfo.iMaxSp)
			m_tStatus.iMp = m_tPlayerInfo.iMaxSp;

		m_tStatus.iHp += 1;
		if (m_tStatus.iHp >= m_tPlayerInfo.iMaxSp)
			m_tStatus.iHp = m_tPlayerInfo.iMaxSp;

		CDataSubject::Get_Instance()->Notify(STATUS, &m_tStatus);
		CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);
	}
}

void CPlayer::Reset_On_Array(void)
{
	for (int i = 0; i < ON_END; ++i)
	{
		m_bIsOn[i] = false;
	}
}

void CPlayer::Eat_item(CItem * pItem)
{
	for (size_t i = 0; i < m_vecInven.size(); ++i)
	{
		if (m_vecInven[i] != nullptr)
		{
			if (m_vecInven[i]->Get_ItemInfo().wstrObjKey == pItem->Get_ItemInfo().wstrObjKey)
			{
				m_vecInven[i]->Get_ItemInfo().iCount += pItem->Get_ItemInfo().iCount;
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
				CSoundMgr::Get_Instance()->PlaySound(L"Drag.wav", CSoundMgr::PLAYER);
				return;
			}
		}

	}

	//for (size_t i = 0; i < m_vecInven.size(); ++i)
	//{
	//	if (m_vecInven[i] == nullptr)
	//	{
	//		m_vecInven[i] = pItem;
	//	}
	//	return;
	//}

	m_vecInven.push_back(pItem);
	CSoundMgr::Get_Instance()->PlaySound(L"Drag.wav", CSoundMgr::PLAYER);
}

void CPlayer::Use_Item(CItem * pItem)
{
	ITEM_INFO tItem = pItem->Get_ItemInfo();

	if (tItem.iCount >= 0)
	{
		switch (tItem.eItemID)
		{
		case IT_HEAD:
		case IT_ARMOR:
		case IT_ARMS:
		case IT_HAND:
			CSoundMgr::Get_Instance()->PlaySound(L"SwordEquip.WAV", CSoundMgr::PLAYER);
		case IT_FOOT:
		{
			CItem* pOldItem = m_arrEquip[tItem.eItemID];
			if (pOldItem != nullptr)
				m_vecInven.push_back(pOldItem);

			CEquipment* pEquipment = static_cast<CEquipment*>(pItem);
			m_arrEquip[tItem.eItemID] = pEquipment;
			PLAYER_INFO tBuff = pEquipment->Get_Buff();

			// 장비 아이템 버프
			m_tPlayerInfo.iStab += tBuff.iStab;
			m_tPlayerInfo.iHack += tBuff.iHack;
			m_tPlayerInfo.iInt += tBuff.iInt;
			m_tPlayerInfo.iDef += tBuff.iDef;
			m_tPlayerInfo.iMr += tBuff.iMr;
			m_tPlayerInfo.iDex += tBuff.iDex;
			m_tPlayerInfo.iAgi += tBuff.iAgi;

			CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);
		}
		break;

		case IT_POTION:
		{
			CPotion* pPotion = static_cast<CPotion*>(pItem);
			int iBuff = pPotion->Get_Buff();
			switch (pPotion->Get_Category())
			{
			case HP:
				m_tStatus.iHp += iBuff;
				if (m_tStatus.iHp >= m_tStatus.iMaxHp) 
					m_tStatus.iHp = m_tStatus.iMaxHp;
				break;

			case MP:
				m_tStatus.iMp += iBuff;
				if (m_tStatus.iMp >= m_tStatus.iMaxMp)
					m_tStatus.iMp = m_tStatus.iMaxMp;
				break;

			case SP:
				m_tPlayerInfo.iSp += iBuff;
				if (m_tPlayerInfo.iSp >= m_tPlayerInfo.iMaxSp)
					m_tPlayerInfo.iSp = m_tPlayerInfo.iMaxSp;
				break;
			}

			CEffect* pEffect = new CPotionEffect(pPotion->Get_Category());
			pEffect->Initialize();
			CObjMgr::Get_Instance()->AddObject(OBJ_EFFECT, pEffect);

			CSoundMgr::Get_Instance()->PlaySound(L"PotionUse.wav", CSoundMgr::PLAYER);

			CDataSubject::Get_Instance()->Notify(STATUS, &m_tStatus);
			CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);

			pItem->Get_ItemInfo().iCount--;
		}
		break;
		}
	}
}

void CPlayer::UnEqiup_Item(CEquipment * pItem)
{
	int iID = pItem->Get_ItemInfo().eItemID;
	m_arrEquip[iID] = nullptr;

	if (pItem->Get_ItemInfo().eItemID == IT_HAND)
		CSoundMgr::Get_Instance()->PlaySound(L"SwordEquip.WAV", CSoundMgr::PLAYER);

	PLAYER_INFO tBuff = pItem->Get_Buff();

	m_tPlayerInfo.iStab -= tBuff.iStab;
	m_tPlayerInfo.iHack -= tBuff.iHack;
	m_tPlayerInfo.iInt -= tBuff.iInt;
	m_tPlayerInfo.iDef -= tBuff.iDef;
	m_tPlayerInfo.iMr -= tBuff.iMr;
	m_tPlayerInfo.iDex -= tBuff.iDex;
	m_tPlayerInfo.iAgi -= tBuff.iAgi;
	
	CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);

	Eat_item(pItem);
}

void CPlayer::Select_Skill(wstring wstrSkill)
{
	m_wstrSkill = wstrSkill;
	CMouse* pMouse = static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
	pMouse->Set_State(CMouse::ON_SKILL, true);
}

void CPlayer::Cancel_Skill()
{
	m_wstrSkill = L"";
	CMouse* pMouse = static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
	pMouse->Cancel_Skill();
}

void CPlayer::Use_Skill()
{
	POINT ptMouse = GetMousePt();

	if (m_tStatus.iMp >= 20)
	{
		m_tStatus.iMp -= 20;

		if (m_wstrSkill == L"Healing")
		{
			CEffectExtinction* pEffect = new CEffectExtinction(L"Effect", L"Healing");
			pEffect->Initialize();
			CObjMgr::Get_Instance()->AddObject(OBJ_EFFECT, pEffect);

			int iBuff = rand() % 20 + 10; // 10~30
			m_tStatus.iHp += iBuff;
			if (m_tStatus.iHp >= m_tStatus.iMaxHp)
				m_tStatus.iHp = m_tStatus.iMaxHp;

			CDataSubject::Get_Instance()->Notify(STATUS, &m_tStatus);
		}

		if (m_wstrSkill == L"Shield")
		{
			CEffectBuff* pEffect = new CEffectBuff(L"Effect", L"Shield", 10.f);
			pEffect->Initialize();
			CObjMgr::Get_Instance()->AddObject(OBJ_EFFECT, pEffect);
			Set_Shield(true);
		}

		if (m_wstrSkill == L"BotFire")
		{
			
		}

		if (m_wstrSkill == L"Freeze")
		{
			
		}


		if (m_wstrSkill == L"MultiAttack")
		{

		}


		if (m_wstrSkill == L"PurpleFog")
		{

		}


		if (m_wstrSkill == L"ThunderBolt")
		{

		}

	}
	else
	{
		CSoundMgr::Get_Instance()->PlaySound(L"NoMana.MP3", CSoundMgr::PLAYER);
	}
	Cancel_Skill();
}

void CPlayer::Buy_Item(CItem * pItem)
{
	if (nullptr == pItem)
		return;

	wstring wstrObjKey = pItem->Get_ItemInfo().wstrObjKey;
	ITEMID eID = pItem->Get_ItemInfo().eItemID;
	if(eID == IT_POTION)
		pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(wstrObjKey));
	else
		pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(wstrObjKey));

	int iCost = pItem->Get_ItemInfo().iCost;

	// 돈이 없으면 구매하지 못함
	if (m_tPlayerInfo.iSeed < iCost)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
		CSoundMgr::Get_Instance()->PlaySound(L"NoMana.MP3", CSoundMgr::UI);
	}
	// 돈이 있다면 
	else
	{
		m_tPlayerInfo.iSeed -= iCost;
		
		bool bAlreay = false;
		
		for (int i = 0; i < m_vecInven.size(); ++i)
		{
			if (m_vecInven[i] == nullptr)
				continue;

			if (m_vecInven[i]->Get_ItemInfo().wstrObjKey == pItem->Get_ItemInfo().wstrObjKey)
			{
				m_vecInven[i]->Get_ItemInfo().iCount++;
				bAlreay = true;
				break;
			}
		}
	
		if(!bAlreay)
			m_vecInven.push_back(pItem);
		
		CDataSubject::Get_Instance()->Notify(PLAYER_INVENTORY, &m_vecInven);
		CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
		CSoundMgr::Get_Instance()->PlaySound(L"Buy.wav", CSoundMgr::UI);
	}
}

void CPlayer::Sell_Item(CItem * pItem)
{
	if (nullptr == pItem)
		return;

	int iCost = pItem->Get_ItemInfo().iCost;
	pItem->Get_ItemInfo().iCount--;
	// 80% 가격으로 판매
	m_tPlayerInfo.iSeed += iCost*0.8;

	CDataSubject::Get_Instance()->Notify(PLAYER_INVENTORY, &m_vecInven);
	CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);

	CSoundMgr::Get_Instance()->PlaySound(L"Sell.wav", CSoundMgr::UI);
}

void CPlayer::Get_Damage(CBattleUnit * pAttacker)
{
	m_bIsOn[ON_DAMAGE] = true;
	m_fDamageTime = 0.f;
	
	m_vNutBackDir = this->m_tInfo.vPos - pAttacker->Get_Info().vPos;
	D3DXVec3Normalize(&m_vNutBackDir, &m_vNutBackDir);
	m_vMoveDest = { 0.f, 0.f, 0.f };

	m_tInfo.vDir = pAttacker->Get_Info().vPos - this->m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

void CPlayer::Attack(CBattleUnit * pTarget, int iDamage)
{
	if (!m_bIsDead)
	{
		STATUS_INFO& tTargetStatus = pTarget->Get_Status();

		m_tInfo.vDir = pTarget->Get_Info().vPos - m_tInfo.vPos;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		m_bIsOn[ON_WALK] = false;

		if (pTarget->Is_Shield())
		{
			// todo 할일 확인
			// 미스 이펙트 띄우기
			return;
		}

		// 플레이어가 크리티컬을 잘 때리는 것은 agi 가 높아서임
		int iAgi = this->m_tPlayerInfo.iAgi;
		int iPercent = 0;
		if (iAgi < 31) iPercent = 5;
		else if (iAgi < 32) iPercent = 6;
		else if (iAgi < 33) iPercent = 7;
		int iCritical = rand() % 10;
		if (iCritical < iPercent)
			iDamage *= 1.5f;

		tTargetStatus.iHp -= iDamage;
		if (tTargetStatus.iHp <= 0)
		{
			pTarget->Set_Dead(true);
			CMonster* pMonster = static_cast<CMonster*>(pTarget);

			int iReturnExp = pMonster->Get_ReturnExp();
			//wstring wstrItem = pMonster->Get_ReturnItem();

			CMenuUI* pMenu = static_cast<CMenuUI*>(CObjMgr::Get_Instance()->Get_MenuUI());
			CEventWnd* pEventWnd = static_cast<CEventWnd*>(pMenu->Get_UI(CMenuUI::PT_EVENT));
			pEventWnd->Up_MonsterKill();

			// 레벨 업
			m_tPlayerInfo.iExp += iReturnExp;
			if (m_tPlayerInfo.iExp >= m_tPlayerInfo.iMaxExp)
			{
				Level_Up(true);
			}

			CDataSubject::Get_Instance()->Notify(STATUS, &m_tStatus);
			CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);


			//// 아이템 떨구기
			//if (wstrItem != L"" && iCritical < iPercent)
			//{
			//	CItem* pItem = new CItem(CItemMgr::Get_Instance()->Create_Potion(wstrItem));
			//	pItem->Set_Pos(pTarget->Get_Info().vPos);
			//	CBack* pBack = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background());
			//	pBack->Push_Item(pItem);
			//}
		}

		CDamage* pDamage = new CDamage(iDamage);
		pDamage->Set_Pos(pTarget->Get_Info().vPos);
		pDamage->Initialize();
		CObjMgr::Get_Instance()->AddObject(OBJ_EFFECT, pDamage);

		if (iCritical < iPercent)
			pDamage->Critical(true);
		else
			pDamage->Critical(false);

		if (m_pWeapon != nullptr)
		{
			static int i = 0;
			i++;

			if (i % 3 == 0)
			{
				m_bIsOn[ON_HORCUT] = true;
				m_bIsOn[ON_VERCUT] = false;
				m_bIsOn[ON_STAB] = false;
			}
			else if (i % 3 == 1)
			{
				m_bIsOn[ON_HORCUT] = false;
				m_bIsOn[ON_VERCUT] = true;
				m_bIsOn[ON_STAB] = false;
			}
			else
			{
				m_bIsOn[ON_HORCUT] = false;
				m_bIsOn[ON_VERCUT] = false;
				m_bIsOn[ON_STAB] = true;
			}

			if (i % 2 == 0)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
				CSoundMgr::Get_Instance()->PlaySound(L"firstAtt.wav", CSoundMgr::PLAYER);
			}
			else
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
				CSoundMgr::Get_Instance()->PlaySound(L"SecondAtt.wav", CSoundMgr::PLAYER);
			}
		}

		pTarget->Get_Damage(this);

		CObjMgr::Get_Instance()->AddObject(OBJ_UI, CObjFactory<CCoolEffect>::CreateObj());
	}
}

void CPlayer::Level_Up(bool bOverMaxExp)
{
	m_tPlayerInfo.iLevel++;

	if(bOverMaxExp)
		m_tPlayerInfo.iExp -= m_tPlayerInfo.iMaxExp;
	else
		m_tPlayerInfo.iExp -= m_tPlayerInfo.iLevelExp;

	m_tStatus.iMaxHp += 30;
	m_tStatus.iMaxMp += 30;
	m_tPlayerInfo.iMaxSp += 30;

	m_tStatus.iHp = m_tStatus.iMaxHp;
	m_tStatus.iMp = m_tStatus.iMaxMp;
	m_tPlayerInfo.iSp = m_tPlayerInfo.iMaxSp;

	m_tPlayerInfo.iMaxExp += 30;
	m_tPlayerInfo.iLevelExp = m_tPlayerInfo.iMaxExp * 2 / 3;

	CEffectExtinction* pEffect = new CEffectExtinction(L"Effect", L"LevelUp");
	pEffect->Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_EFFECT, pEffect);
	m_tPlayerInfo.iPoint += 3;

	CDataSubject::Get_Instance()->Notify(STATUS, &m_tStatus);
	CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
	CSoundMgr::Get_Instance()->PlaySound(L"lvup.MP3", CSoundMgr::PLAYER);
}

void CPlayer::Dead(void)
{
	m_bIsDead = true;
	CEffectExtinction* pEffect = new CEffectExtinction(L"Effect", L"Damage");
	pEffect->Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_EFFECT, pEffect);

	m_tFrame.wstrObjKey = L"Player";
	m_tFrame.wstrStateKey = L"Ghost";
	
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, 0);

	SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

	m_tFrame.iSpriteCnt = 0;
	m_tFrame.fFrameTime = 0;
	m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
	m_tFrame.iSpriteMax = tSprite_Info.iCount;

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlaySound(L"pain.wav", CSoundMgr::PLAYER);
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Boss.mp3");
}

void CPlayer::Revive(void)
{
	m_bIsDead = false;

	CEffectExtinction* pEffect = new CEffectExtinction(L"Effect", L"MagicHit");
	pEffect->Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_EFFECT, pEffect);

	m_tFrame.wstrObjKey = L"Lucian";
	m_tFrame.wstrStateKey = L"Stand_";
	m_wstrDir = L"D";
	m_tInfo.vDir = { 0.f, 1.f, 0.f };
	m_eMove = MS_WALK;
	m_eCurState = S_STAND;
	m_eCurDir = D_DOWN;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey + m_wstrDir, 0);

	if (nullptr == pTexInfo)
		return;

	SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

	m_tFrame.iSpriteCnt = 0;
	m_tFrame.fFrameTime = 0;
	m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
	m_tFrame.iSpriteMax = tSprite_Info.iCount;

	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlaySound(L"PotionUse.wav", CSoundMgr::PLAYER);
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Store.wav");
}

void CPlayer::Cure_From_Npc(void)
{
	m_tStatus.iHp = m_tStatus.iMaxHp;
	m_tStatus.iMp = m_tStatus.iMaxMp;
	m_tPlayerInfo.iSp = m_tPlayerInfo.iMaxSp;
	
	CEffect* pEffect = new CPotionEffect(POTIONID::HP);
	pEffect->Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_EFFECT, pEffect);

	CSoundMgr::Get_Instance()->PlaySound(L"PotionUse.wav", CSoundMgr::PLAYER);

	CDataSubject::Get_Instance()->Notify(STATUS, &m_tStatus);
	CDataSubject::Get_Instance()->Notify(PLAYER_STATUS, &m_tPlayerInfo);
}

void CPlayer::Astar_Start()
{
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(g_hWnd, &mouse);

	mouse.x += LONG(m_vScroll.x);
	mouse.y += LONG(m_vScroll.y);

	m_vMoveDest = { (float)mouse.x, (float)mouse.y, 0.f };

	CBack* pBackground = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background());
	m_iDst_Idx = pBackground->Get_Tile_Index(m_vMoveDest);
	m_iCur_Idx = pBackground->Get_Tile_Index(m_tInfo.vPos);

	//if (m_iDst_Idx != m_iCur_Idx)
	//{
	//	CAstar::Get_Instance()->Astar_Start(m_iCur_Idx, m_iDst_Idx);
	//	m_BestList = CAstar::Get_Instance()->Get_BestList();
	//}
}
