#pragma once
#include "Observer.h"
#include "Player.h"
class CUIObserver :
	public CObserver
{
public:
	CUIObserver();
	virtual ~CUIObserver();

public:
	const STATUS_INFO*			Get_StatusInfo(void) { return &m_tObserverStatus; }
	PLAYER_INFO*				Get_PlayerInfo(void) { return m_pPlayerStatus; }
	const D3DXMATRIX*			Get_ObserverMatrix(void) { return &m_matObserver; }
	const CPlayer::MOVE_STATE	Get_PlayerMoveState(void) { return m_ePlayerMoveState; }
	vector<CItem*>*				Get_Inventory(void) { return m_vecPlayerInven; }
	DIR							Get_ObserverDirection(void) { return m_ObserverDir; }

public:
	virtual void Update(int iFlag, void* pData) override;

private:
	STATUS_INFO			m_tObserverStatus;
	PLAYER_INFO*		m_pPlayerStatus;
	D3DXMATRIX			m_matObserver;
	CPlayer::MOVE_STATE m_ePlayerMoveState;
	vector<CItem*>*		m_vecPlayerInven;
	DIR					m_ObserverDir;
};

