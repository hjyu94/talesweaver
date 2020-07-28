#pragma once
#include "Item.h"
class CEquipment :
	public CItem
{
public:
	CEquipment();
	CEquipment(ITEM_INFO tItemInfo, PLAYER_INFO tStatusBuff);
	virtual ~CEquipment();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	void			Set_eEquip(EQUIP eEquip) { m_eEquip = eEquip; }
	
public:
	EQUIP			Get_EquipCategory(void) { return m_eEquip; }
	PLAYER_INFO		Get_Buff(void) { return m_tStatusBuff; }

private:
	EQUIP			m_eEquip;
	PLAYER_INFO		m_tStatusBuff;
};

