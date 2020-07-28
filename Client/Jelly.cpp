#include "stdafx.h"
#include "Jelly.h"
#include "ObjMgr.h"
#include "DataSubject.h"
#include "ObjFactory.h"

CJelly::CJelly()
	: m_fDamageRenderTime(0.f)
{
}


CJelly::~CJelly()
{
}

HRESULT CJelly::Initialize(void)
{
	m_tFrame.wstrObjKey = L"Jelly";
	m_tFrame.wstrStateKey = L"Walk";
	m_wstrDir = L"LD";

	m_eCurState = S_WALK;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey + m_wstrDir, 0);

	if (nullptr == pTexInfo)
		return E_FAIL;

	SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

	m_tFrame.iSpriteCnt = 0;
	m_tFrame.fFrameTime = 0;
	m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
	m_tFrame.iSpriteMax = tSprite_Info.iCount;

	m_fSpeed = 30.f;
	
	m_tStatus.iMaxHp = 30.f;
	m_tStatus.iHp = m_tStatus.iMaxHp;

	//m_pHPBar = CObjFactory<CHPBar>::CreateObj();
	//CObjMgr::Get_Instance()->AddObject(OBJ_UI, m_pHPBar);

	//CDataSubject::Get_Instance()->AddData(STATUS, &m_tStatus);
	//CDataSubject::Get_Instance()->AddData(PLAYER_MATRIX, &m_tInfo.matWorld);

	m_iAtt = 10;
	m_iReturnExp = 30;

	return S_OK;
}
	
int CJelly::Progress(void)
{
	//CDataSubject::Get_Instance()->Notify(PLAYER_MATRIX, &m_tInfo.matWorld);

	int iReturn = CMonster::Progress();
	if (iReturn == OBJ_DEAD)
		return OBJ_DEAD;

	IsNearPlayer();
	IsAttacking();
	Move();
	Update_Direction();
	Before_Update_State();
	Update_State();
	Update_World_Matrix();
	Move_Frame();
	
	if (m_tStatus.iHp <= 0)
	{
		return OBJ_DEAD;
	}
	
	return 0;
}

void CJelly::Render(void)
{
	CMonster::Render();

	const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey + m_wstrDir, m_tFrame.iSpriteCnt
	);

	float fX = float(pTexture->tImgInfo.Width) * 0.5f + pTexture->tToolInfo.ptCenter.x;
	float fY = float(pTexture->tImgInfo.Height) * 0.5f + pTexture->tToolInfo.ptCenter.x;

	D3DXVECTOR3 vPos = m_tInfo.vPos - m_vScroll;
	m_tRect = Calculate_Rect(vPos, float(pTexture->tImgInfo.Width), float(pTexture->tImgInfo.Height));

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	CGraphicDev::Get_Instance()->GetSprite()->Draw(
		pTexture->pTexture
		, NULL
		, &D3DXVECTOR3(fX, fY, 0.f)
		, NULL
		, D3DCOLOR_ARGB(255, 255, 255, 255) // 모든 색상을 출력
	);

	// 데미지 출력
	CMonster::Render();

	// 디버깅
	CGraphicDev::Get_Instance()->Draw_Rect(m_tRect);
}

void CJelly::Release(void)
{
}

void CJelly::Update_State(void)
{
	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case S_ATTACK:
			m_tFrame.wstrStateKey = L"Attack";
			break;
		case S_DAMAGE:
			m_tFrame.wstrStateKey = L"Damage";
			break;
		case S_DIE:
			m_tFrame.wstrStateKey = L"Die";
			break;
		case S_STAND:
			m_tFrame.wstrStateKey = L"Stand";
			break;
		case S_WALK:
			m_tFrame.wstrStateKey = L"Walk";
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


	if (m_eCurDir != m_eNextDir)
	{
		switch (m_eNextDir)
		{
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


void CJelly::Move_Frame(void)
{
	m_tFrame.fFrameTime += GET_TIME;

	if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
	{
		m_tFrame.fFrameTime = 0;
		m_tFrame.iSpriteCnt++;
	}

	if (m_tFrame.iSpriteCnt >= m_tFrame.iSpriteMax)
	{
		if (m_eCurState == S_ATTACK)
		{
			m_bState[B_ATTACK] = false;
		}

		if (m_eCurState == S_DAMAGE)
		{
			m_bState[B_DAMAGED] = false;
		}

		m_tFrame.iSpriteCnt = 0;
	}
}

void CJelly::Before_Update_State(void)
{
	if (m_bState[B_DAMAGED])
	{
		m_eNextState = S_DAMAGE;
		m_tInfo.vPos += m_vNutBackDir * GET_TIME * 30.f;
	}
	else if (m_bState[B_ATTACK])
	{
		m_eNextState = S_ATTACK;
	}
	else if (m_bState[B_WALK])
	{
		m_eNextState = S_WALK;
	}
	else
	{
		m_eNextState = S_STAND;
	}
}

