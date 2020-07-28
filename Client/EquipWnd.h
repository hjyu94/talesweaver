#pragma once
#include "WndUI.h"
#include "Equipment.h"

class CEquipWnd :
	public CWndUI
{
public:
	CEquipWnd();
	virtual ~CEquipWnd();

public:
	// CWndUI을(를) 통해 상속됨
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	vector<CItem*>*		m_InvenPtr;
	CEquipment**		m_EquipPtr;
	//CItem*				m_Equip[EQ_END];

	D3DXVECTOR3			m_arrItemPos[EQ_END];
	RECT				m_arrItemRect[EQ_END];
	
	D3DXVECTOR3			m_arrNamePos[EQ_END];
	RECT				m_arrNameRect[EQ_END];
	wstring				m_wstrName[EQ_END];

	float				m_fClickTime;
};

