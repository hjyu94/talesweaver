#pragma once
#include "UI.h"
#include "Item.h"
class CQuickSlot :
	public CUI
{
public:
	CQuickSlot();
	virtual ~CQuickSlot();

public:
	enum SLOT{SL_1, SL_2, SL_END};

private:
	float	m_fSlotBtn_Width = 40.f;
	float	m_fSlotBtn_Height = 18.f;
	float	m_fItemBtn_Width = 24.f;
	float	m_fItemBtn_Height = 20.f;


	SLOT	m_eSlot;
	RECT	m_rcSlot[SL_END];

	CItem*	m_arrItem[SL_END][8];
	D3DXVECTOR3	m_vItemPos[8];
	RECT	m_rcItem[8];

	
public:
	// CUI을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
};

