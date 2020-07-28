#include "stdafx.h"
#include "Equipment.h"


CEquipment::CEquipment()
{
}

CEquipment::CEquipment(ITEM_INFO tItemInfo, PLAYER_INFO tStatusBuff)
	: CItem(tItemInfo)
{
	m_tStatusBuff = tStatusBuff;
}


CEquipment::~CEquipment()
{
}

HRESULT CEquipment::Initialize(void)
{
	return E_NOTIMPL;
}

int CEquipment::Progress(void)
{
	CItem::Progress();
	return 0;
}

void CEquipment::Render(void)
{
	CItem::Render();
}

void CEquipment::Release(void)
{
}

