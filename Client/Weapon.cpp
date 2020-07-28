#include "stdafx.h"
#include "Weapon.h"

#include "DataSubject.h"
#include "UIObserver.h"

CWeapon::CWeapon()
{
}

CWeapon::CWeapon(wstring wstrObjKey)
{
	m_tFrame.wstrObjKey = wstrObjKey;
}

CWeapon::~CWeapon()
{
	Release();
}

HRESULT CWeapon::Initialize(void)
{
	m_tFrame.wstrStateKey = L"Stand_";
	m_wstrDir = L"D";

	m_eCurState = S_STAND;
	m_eCurDir = D_DOWN;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey + m_wstrDir, 0);

	if (nullptr == pTexInfo)
		return E_FAIL;

	SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

	m_tFrame.iSpriteCnt = 0;
	m_tFrame.fFrameTime = 0;
	m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
	m_tFrame.iSpriteMax = tSprite_Info.iCount;

	m_pObserver = new CUIObserver;
	CDataSubject::Get_Instance()->Subscribe(m_pObserver);

	return S_OK;
}

int CWeapon::Progress(void)
{
	Update_Direction();
	Update_State();
	Update_World_Matrix();
	Move_Frame();
	return 0;
}

void CWeapon::Render(void)
{
	if (m_bShow)
	{
		m_tFrame.iSpriteCnt = m_pOwner->Get_Frame().iSpriteCnt;

		const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
			m_tFrame.wstrObjKey, m_tFrame.wstrStateKey + m_wstrDir, m_tFrame.iSpriteCnt
		);

		float fCX = float(pTexture->tImgInfo.Width) * 0.5f + pTexture->tToolInfo.ptCenter.x;
		float fCY = float(pTexture->tImgInfo.Height) * 0.5f + pTexture->tToolInfo.ptCenter.x;


		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

		CGraphicDev::Get_Instance()->GetSprite()->Draw(
			pTexture->pTexture
			, NULL
			, &D3DXVECTOR3(fCX, fCY, 0.f)
			, NULL
			, D3DCOLOR_ARGB(255, 255, 255, 255) // 모든 색상을 출력
		);
	}
}

void CWeapon::Release(void)
{
	Safe_Delete(m_pObserver);
}

void CWeapon::Update_World_Matrix(void)
{
	D3DXMATRIX	matTrans, matScale, matParent;

	const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey + m_wstrDir, m_tFrame.iSpriteCnt
	);

	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

	if (pTexture->tToolInfo.bReverse)
	{
		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	}
	else
	{
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	}

	matParent = *dynamic_cast<CUIObserver*>(m_pObserver)->Get_ObserverMatrix();

	m_tInfo.matWorld = matScale * matTrans * matParent;
}

void CWeapon::Update_State(void)
{
	CPlayer::STATE eState = m_pOwner->Get_CurState();
	switch (eState)
	{
	case CPlayer::S_STAND:
		m_eNextState = S_STAND;
		break;
	case CPlayer::S_HORCUT:
		m_eNextState = S_HORCUT;
		break;
	case CPlayer::S_STAB:
		m_eNextState = S_STAB;
		break;
	case CPlayer::S_VERCUT:
		m_eNextState = S_VERCUT;
		break;
	}

	if (m_eCurState != m_eNextState)
	{
		switch (m_eNextState)
		{
		case S_HORCUT:
			m_tFrame.wstrStateKey = L"HorCut_";
			break;

		case S_STAB:
			m_tFrame.wstrStateKey = L"Stab_";
			break;

		case S_STAND:
			m_tFrame.wstrStateKey = L"Stand_";
			break;

		case S_VERCUT:
			m_tFrame.wstrStateKey = L"VerCut_";
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

void CWeapon::Update_Direction(void)
{
	m_eNextDir = static_cast<CUIObserver*>(m_pObserver)->Get_ObserverDirection();
}

void CWeapon::Late_Progress(void)
{
}
