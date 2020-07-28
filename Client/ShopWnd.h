#pragma once
#include "WndUI.h"
#include "Item.h"
class CNpc;
class CShopWnd :
	public CWndUI
{
public:
	CShopWnd();
	virtual ~CShopWnd();

public:
	// CWndUI을(를) 통해 상속됨
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	void			Set_Owner(CNpc* pNpc);

public:
	enum SCROLLID { S_UP, S_SCROLL_MIN, S_SCROLL_MAX, S_DOWN, S_END };

private:
	D3DXVECTOR3		m_vImgPos[12];
	RECT			m_ImgRect[12];

	D3DXVECTOR3		m_vNamePos[12]; // Left-top
	RECT			m_NameRect[12];

	D3DXVECTOR3		m_vCostPos[12]; // Left-top
	RECT			m_CostRect[12];

	vector<CItem*>*	m_pShopInven;
	
	bool			m_bScrollNeed = false;
	int				m_iScrollStart = 0;
	D3DXVECTOR3		m_vecScroll[S_END];
	RECT			m_rcScroll[S_END];

	float			m_fClickTime = 0.f;
};

