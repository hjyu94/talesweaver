#pragma once
#include "WndUI.h"
#include "Item.h"

class CInvenWnd :
	public CWndUI
{
public:
	CInvenWnd();
	virtual ~CInvenWnd();
	
public:
	// CWndUI을(를) 통해 상속됨
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	
private:
	D3DXVECTOR3		m_vImgPos[12];
	RECT			m_rcImg[12];

	D3DXVECTOR3		m_vNamePos[12]; // Left-top
	RECT			m_rcName[12];
	
	D3DXVECTOR3		m_vCountPos[12]; // Left-top
	RECT			m_rcCount[12];

	D3DXVECTOR3		m_vSeedPos;
	RECT			m_rcSeed;
	TCHAR			m_szSeed[MIN_STR];

	vector<CItem*>*	m_pInven;
	wstring			m_wstrName[12];
	TCHAR			m_szCount[12][MIN_STR];

	float			m_fClickTime;
};

