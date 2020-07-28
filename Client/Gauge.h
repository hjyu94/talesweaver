#pragma once
#include "UI.h"
#include "Player.h"

class CGauge :
	public CUI
{
public:
	CGauge();
	virtual ~CGauge();

public:
	// CUI을(를) 통해 상속됨
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	D3DXVECTOR3 m_vWndPos;
	D3DXVECTOR3 m_vHPBarPos;
	D3DXVECTOR3 m_vMPBarPos;
	D3DXVECTOR3 m_vSPBarPos;
	D3DXVECTOR3 m_vIconPos;

	int		m_iHp = 0;
	int		m_iMp = 0;
	int		m_iSp = 0;

	int		m_iMaxHp = 0;
	int		m_iMaxMp = 0;
	int		m_iMaxSp = 0;

	CPlayer::MOVE_STATE m_eMove;
};

