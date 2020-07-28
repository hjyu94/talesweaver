#pragma once
#include "WndUI.h"
class CStatusWnd :
	public CWndUI
{
public:
	CStatusWnd();
	virtual ~CStatusWnd();

public:
	// CWndUI을(를) 통해 상속됨
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	PLAYER_INFO*		m_pPlayerInfo;
	STATUS_INFO			m_tStatus;

	D3DXVECTOR3			m_vPos[24];
	RECT				m_rcData[24];
	TCHAR				m_szData[24][MID_STR];
	
	// 칭호 (추가함)
	D3DXVECTOR3			m_vNickNamePos;
	RECT				m_rcNickName;

	D3DXVECTOR3			m_vExpBarPos;
	D3DXVECTOR3			m_vMaxBtnPos;
	RECT				m_rcMaxBtn;

	D3DXVECTOR3			m_vPlus[7];
	int					m_iPoint[7];
	RECT				m_rcPlus[7];
};

