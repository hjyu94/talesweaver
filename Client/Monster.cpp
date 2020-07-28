#include "stdafx.h"
#include "Monster.h"
#include "ObjMgr.h"
#include "DataSubject.h"
#include "UIObserver.h"

CMonster::CMonster()
	: m_fDirSetTime(0.f)
	, m_fMoveTime(0.f)
	, m_fAttackTime(0.f)
{
	for (int i = 0; i < B_END; ++i)
	{
		m_bState[i] = false;
	}
}


CMonster::~CMonster()
{
	Release();
}

HRESULT CMonster::Initialize(void)
{
	return E_NOTIMPL;
}

int CMonster::Progress(void)
{
	POINT pt = GetMousePt();
	if (PtInRect(&m_tRect, pt))
	{
		if (CKeyMgr::Get_Instance()->KeyPressed(KEY_LBUTTON, CKeyMgr::S_MONSTER))
		{
			// Todo
			static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_Target(this);
		}
			static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse())->Set_State(CMouse::ON_MONSTER, true);
	}

	if (m_bIsDead)
	{
		static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse())->Set_State(CMouse::ON_MONSTER, false);
		return OBJ_DEAD;
	}
	return 0;
}

void CMonster::Render(void)
{
	const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey + m_wstrDir, m_tFrame.iSpriteCnt
	);

	float fCY = pTexture->tImgInfo.Height;

	// HP Bar
	float fScale = float(m_tStatus.iHp) / m_tStatus.iMaxHp;

	D3DXMATRIX		matTrans, matScale, matWorld;
	
	D3DXMatrixTranslation(&matTrans
		, m_tInfo.vPos.x - m_vScroll.x - 22.f
		, m_tInfo.vPos.y - m_vScroll.y + fCY *0.5f + 10.f
		, 0.f);

	D3DXMatrixScaling(&matScale, fScale, 1.f, 0.f);

	matWorld = matScale * matTrans;

	pTexture = CTextureMgr::Get_Instance()->GetTexture(L"MonsterHpBar");

	if (nullptr == pTexture)
		return;

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);

	CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
		NULL,
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMonster::Release(void)
{

}

void CMonster::Update_World_Matrix(void)
{
	D3DXMATRIX matTrans, matScale;

	D3DXMatrixTranslation(
		&matTrans
		, m_tInfo.vPos.x - m_vScroll.x
		, m_tInfo.vPos.y - m_vScroll.y
		, 0.f);

	const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey + m_wstrDir, m_tFrame.iSpriteCnt
	);

	if (pTexture->tToolInfo.bReverse)
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	else
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	m_tInfo.matWorld = matScale * matTrans;
}

void CMonster::IsNearPlayer(void)
{
	float fDistance = Get_Distance(m_tInfo.vPos, CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos);

	// 플레이어랑 가까워지면 플레이어에게 다가옴
	if (fDistance < 300.f)
	{
		m_bState[B_NEAR_PLAYER] = true;
	}
	else
	{
		m_bState[B_NEAR_PLAYER] = false;
	}
}

void CMonster::Move(void)
{
	// 1초마다 방향 갱신, 못 가는 타일의 경우 가지 않는다. 
	if (!m_bState[B_NEAR_PLAYER])
	{
		m_fDirSetTime += GET_TIME; // 1.f
		m_fMoveTime += GET_TIME; // 2초 돌아다니고 0.5초 정지, 방향은 1초마다 바꾼다

		if (m_fDirSetTime >= 1.f)
		{
			m_fDirSetTime = 0.f;

			float fX = float(rand() % 10 - 5);
			float fY = float(rand() % 10 - 5);
			D3DXVECTOR3 vDir = { fX, fY, 0.f };
			D3DXVec3Normalize(&m_tInfo.vDir, &vDir);
		}

		if (m_fMoveTime < 2.f)
		{
			CBack* pBack = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background());
			TILE_DRAW eDraw;
			TILE_OPTION eOption;
			pBack->Get_Option_Draw(m_tInfo.vPos, eDraw, eOption);

			m_tInfo.vPos += m_tInfo.vDir * GET_TIME * m_fSpeed;
			m_bState[B_WALK] = true;

			if (TO_NO_MOVE == eOption)
				m_tInfo.vPos -= m_tInfo.vDir * GET_TIME * m_fSpeed * 1.5f;
		}
		else if (m_fMoveTime < 2.5f) // 2 ~ 2.5 초 사이에는 정지
		{
			m_bState[B_WALK] = false;
		}
		else
		{
			m_fMoveTime = 0.f;
		}
	}

	// 플레이어의 위치를 아는 경우 80까지는 직진으로 온다.
	else
	{
		CBattleUnit* pPlayer = static_cast<CBattleUnit*>(CObjMgr::Get_Instance()->Get_Player());
		float fDistance = Get_Distance(m_tInfo.vPos, pPlayer->Get_Info().vPos);
		if (fDistance >= 80)
		{
			m_tInfo.vDir = CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos - m_tInfo.vPos;
			D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
			m_tInfo.vPos += m_tInfo.vDir * GET_TIME * m_fSpeed;
			m_bState[B_WALK] = true;
		}
		else
		{
			m_bState[B_WALK] = false;
		}
	}
}

void CMonster::Update_Direction(void)
{
	if (m_tInfo.vDir.x > 0)
	{
		if (m_tInfo.vDir.y > 0)
		{
			m_eNextDir = D_RD;
		}
		else
		{
			m_eNextDir = D_RU;
		}
	}

	else
	{
		if (m_tInfo.vDir.y > 0)
		{
			m_eNextDir = D_LD;
		}
		else
		{
			m_eNextDir = D_LU;
		}
	}
}

void CMonster::IsAttacking(void)
{
	CBattleUnit* pPlayer = static_cast<CBattleUnit*>(CObjMgr::Get_Instance()->Get_Player());
	float fDistance = Get_Distance(m_tInfo.vPos, pPlayer->Get_Info().vPos);

	// 플레이어랑 가까워지면 2초마다 플레이어 구타
	if (fDistance <= 100.f)
	{
		m_fAttackTime += GET_TIME;
		if (m_fAttackTime > 2.f)
		{
			if (!pPlayer->Is_Dead())
			{
				m_fAttackTime = 0.f;
				Attack(m_iAtt);
				m_bState[B_ATTACK] = true;
			}
		}
	}
}


void CMonster::Get_Damage(CBattleUnit * pAttacker)
{
	m_bState[B_DAMAGED] = true;
	m_fDamageTime = 0;
	m_vNutBackDir = this->m_tInfo.vPos - pAttacker->Get_Info().vPos;
	D3DXVec3Normalize(&m_vNutBackDir, &m_vNutBackDir);
	m_tInfo.vDir = pAttacker->Get_Info().vPos - this->m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
}

void CMonster::Attack(int iDamage)
{
	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	STATUS_INFO& tStatus = pPlayer->Get_Status();
	PLAYER_INFO& pPlayerStatus = pPlayer->Get_PlayerStatus();

	m_tInfo.vDir = pPlayer->Get_Info().vPos - m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	if (pPlayer->Is_Shield())
	{
		// todo 할일 확인
		// 미스 이펙트 띄우기
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
		CSoundMgr::Get_Instance()->PlaySound(L"miss.wav", CSoundMgr::MONSTER);
		return;
	}

	// 플레이어의 Def가 높을수록 덜 쎄게 맞음
	int iDef = pPlayerStatus.iDef;
	if (iDef < 31) iDamage -= 1;
	else if (iDef < 32) iDamage -= 3;
	else if (iDef < 33) iDamage -= 5;
	if (iDamage < 0) iDamage = 0;

	// 플레이어가 크리티컬로 맞는것은 Dex가 낮아서임
	int iDex = pPlayerStatus.iDex;
	int iPercent = 0;
	if (iDex < 31) iPercent = 5;
	else if (iDex < 32) iPercent = 6;
	else if (iDex < 33) iPercent = 7;
	int iCritical = rand() % 10;
	if (iCritical > iPercent)
		iDamage *= 1.5f;

	if(!pPlayer->Is_Powerful())
		tStatus.iHp -= iDamage;

	if (tStatus.iHp <= 0)
	{
		tStatus.iHp = 0;
		pPlayer->Dead();
	}
	
	CDataSubject::Get_Instance()->Notify(STATUS, &tStatus);
	
	CDamage* pDamage = new CDamage(iDamage);
	pDamage->Set_Pos(pPlayer->Get_Info().vPos);
	pDamage->Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_EFFECT, pDamage);

	if (iCritical > iPercent)
		pDamage->Critical(true);
	else
		pDamage->Critical(false);

	pPlayer->Get_Damage(this);
}

