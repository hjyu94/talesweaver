#pragma once
#include "Item.h"
class CPotion :
	public CItem
{
public:
	CPotion();
	CPotion(ITEM_INFO tItemInfo, POTIONID eCategory, int iBuff);
	virtual ~CPotion();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);

public:
	int			Get_Buff() { return m_iBuff; }
	POTIONID	Get_Category() { return m_ePotion; }

private:
	POTIONID	m_ePotion;
	int			m_iBuff;
};

