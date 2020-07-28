#include "stdafx.h"
#include "Potion.h"


CPotion::CPotion()
{
}

CPotion::CPotion(ITEM_INFO tItemInfo, POTIONID eCategory, int iBuff)
	: CItem(tItemInfo)
{
	m_ePotion = eCategory;
	m_iBuff = iBuff;
}


CPotion::~CPotion()
{
}

HRESULT CPotion::Initialize(void)
{
	return E_NOTIMPL;
}

int CPotion::Progress(void)
{
	CItem::Progress();
	return 0;
}

void CPotion::Render(void)
{
	CItem::Render();
}

void CPotion::Release(void)
{
}

void CPotion::Update_World_Matrix(void)
{
}

void CPotion::Update_State(void)
{
}
