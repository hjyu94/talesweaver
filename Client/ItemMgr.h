#pragma once
#include "Include.h"

#include "Potion.h"
#include "Equipment.h"

class CItemMgr
{
	DECLARE_SINGLETON(CItemMgr)

public:
	CItemMgr();
	virtual ~CItemMgr();

public:
	HRESULT			Initialize(void);
	void			Release(void);

	CPotion				Create_Potion(wstring wstrObjKey, int iCount = 1);
	CEquipment			Create_Equip(wstring wstrObjKey, int iCount = 1);

private:
	map<wstring, CItem*>				m_MapItem;

};

