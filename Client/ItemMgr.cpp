#include "stdafx.h"
#include "ItemMgr.h"

IMPLEMENT_SINGLETON(CItemMgr)

CItemMgr::CItemMgr()
{
}


CItemMgr::~CItemMgr()
{
	Release();
}

HRESULT CItemMgr::Initialize(void)
{
	//########## 장비
	//## FOOT
	{	
		wstring wstrObjKey = L"RedShoes";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_FOOT, L"빨간 신발", L"RedShoes", 500);
		
		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iDex = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);
		
		m_MapItem.emplace(wstrObjKey, pEquip);
	}
	
	{
		wstring wstrObjKey = L"LeatherShoes";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_FOOT, L"가죽 신발", L"LeatherShoes", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iDex = 5;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}

	//## HAND
	{
		wstring wstrObjKey = L"RedRing";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_HAND, L"빨간 반지", L"RedRing", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iDef = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}

	{
		wstring wstrObjKey = L"GreenRing";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_HAND, L"초록 반지", L"GreenRing", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iDef = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}

	//## FOOT
	{
		wstring wstrObjKey = L"RedFoot";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_FOOT, L"빨간 발 장신구", L"RedFoot", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iDef = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}

	{
		wstring wstrObjKey = L"GreenFoot";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_FOOT, L"초록 발 장신구", L"GreenFoot", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iDef = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}


	//## ARMOR
	{
		wstring wstrObjKey = L"LightArmor";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_ARMOR, L"Light Armor", L"LightArmor", 500 );

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iDex = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}
	{
		wstring wstrObjKey = L"HeavyArmor";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_ARMOR, L"HeavyArmor", L"HeavyArmor", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iDex = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}

	//## ARMS
	{
		wstring wstrObjKey = L"BloodSwordSingleTex";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_ARMS, L"BloodSword", L"BloodSwordSingleTex", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iHack = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}

	{
		wstring wstrObjKey = L"LightSword";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_ARMS, L"Light Sword", L"LightSword", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iHack = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}
	{
		wstring wstrObjKey = L"MagicWand";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_ARMOR, L"Magic Wand", L"MagicWand", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iInt = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}
	{
		wstring wstrObjKey = L"Mace";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_ARMOR, L"Mace", L"Mace", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iHack = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}
	{
		wstring wstrObjKey = L"Sword";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_ARMOR, L"Sword", L"Sword", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iHack = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}
	{
		wstring wstrObjKey = L"BlueAx";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_ARMOR, L"Blue Ax", L"BlueAx", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iHack = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}
	{
		wstring wstrObjKey = L"DevilSword";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_ARMOR, L"DevilSword", L"DevilSword", 9999999);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iHack = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}

	//## HEAD
	{
		wstring wstrObjKey = L"FabricHood";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_HEAD, L"FabricHood", L"FabricHood", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iDef = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}
	{
		wstring wstrObjKey = L"ClradHat";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_HEAD, L"ClradHat", L"ClradHat", 500);

		PLAYER_INFO	m_tStatusBuff = {};
		//ZeroMemory(&m_tStatusBuff, sizeof(PLAYER_INFO));
		m_tStatusBuff.iDef = 10;

		CEquipment* pEquip = new CEquipment(m_tItemInfo, m_tStatusBuff);

		m_MapItem.emplace(wstrObjKey, pEquip);
	}

	//################# 포션
	//## HP
	{
		wstring wstrObjKey = L"HP100";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"HP100", L"HP100", 150 );
		
		CPotion* pPotion = new CPotion(m_tItemInfo, HP, 100);
		
		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	{
		wstring wstrObjKey = L"HP80";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"HP80", L"HP80", 100 );

		CPotion* pPotion = new CPotion(m_tItemInfo, HP, 80);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	{
		wstring wstrObjKey = L"HP50";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"HP50", L"HP50", 50 );

		CPotion* pPotion = new CPotion(m_tItemInfo, HP, 50);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	{
		wstring wstrObjKey = L"HP30";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"HP30", L"HP30", 30 );

		CPotion* pPotion = new CPotion(m_tItemInfo, HP, 30);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	{
		wstring wstrObjKey = L"HP10";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"HP10", L"HP10", 10 );

		CPotion* pPotion = new CPotion(m_tItemInfo, HP, 10);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}
	
	//## MP
	{
		wstring wstrObjKey = L"MP100";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"MP100", L"MP100", 150 );

		CPotion* pPotion = new CPotion(m_tItemInfo, MP, 100);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	{
		wstring wstrObjKey = L"MP80";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"MP80", L"MP80", 100 );

		CPotion* pPotion = new CPotion(m_tItemInfo, MP, 80);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	{
		wstring wstrObjKey = L"MP50";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"MP50", L"MP50", 50 );

		CPotion* pPotion = new CPotion(m_tItemInfo, MP, 50);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	{
		wstring wstrObjKey = L"MP30";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"MP30", L"MP30", 30 );

		CPotion* pPotion = new CPotion(m_tItemInfo, MP, 30);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	{
		wstring wstrObjKey = L"MP10";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"MP10", L"MP10", 10 );

		CPotion* pPotion = new CPotion(m_tItemInfo, MP, 10);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}


	//## SP
	{
		wstring wstrObjKey = L"SP100";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"SP100", L"SP100", 150 );

		CPotion* pPotion = new CPotion(m_tItemInfo, SP, 100);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	{
		wstring wstrObjKey = L"SP80";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"SP80", L"SP80", 100 );

		CPotion* pPotion = new CPotion(m_tItemInfo, SP, 80);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	{
		wstring wstrObjKey = L"SP50";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"SP50", L"SP50", 50 );

		CPotion* pPotion = new CPotion(m_tItemInfo, SP, 50);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	{
		wstring wstrObjKey = L"SP30";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"SP30", L"SP30", 30 );

		CPotion* pPotion = new CPotion(m_tItemInfo, SP, 30);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	{
		wstring wstrObjKey = L"SP10";

		ITEM_INFO	m_tItemInfo = ITEM_INFO( IT_POTION, L"SP10", L"SP10", 10 );

		CPotion* pPotion = new CPotion(m_tItemInfo, SP, 10);

		m_MapItem.emplace(wstrObjKey, pPotion);
	}

	// ETC
	{
		wstring wstrObjKey = L"RedHub";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_ETC, L"RedHub", L"RedHub", 10);
		CItem* pItem = new CItem(m_tItemInfo);

		m_MapItem.emplace(wstrObjKey, pItem);
	}
	{
		wstring wstrObjKey = L"GreenHub";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_ETC, L"GreenHub", L"GreenHub", 10);
		CItem* pItem = new CItem(m_tItemInfo);

		m_MapItem.emplace(wstrObjKey, pItem);
	}
	{
		wstring wstrObjKey = L"BlueHub";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_ETC, L"BlueHub", L"BlueHub", 10);
		CItem* pItem = new CItem(m_tItemInfo);

		m_MapItem.emplace(wstrObjKey, pItem);
	}
	{
		wstring wstrObjKey = L"ReturnFeather";

		ITEM_INFO	m_tItemInfo = ITEM_INFO(IT_ETC, L"깃털", L"ReturnFeather", 10);
		CItem* pItem = new CItem(m_tItemInfo);

		m_MapItem.emplace(wstrObjKey, pItem);
	}
	/*
		RedRing | ..\Te
		RedHub | ..\Tex
		RedFoot | ..\Te
		MagicWand | ..\
		Mace | ..\Textu
		HeavyArmor | ..
		GreenRing | ..\
		GreenHub | ..\T
		GreenFoot | ..\
		FabricHood | ..
		Empty | ..\Text
		DevilSword | ..
		ClradHat | ..\T
		BlueHub | ..\Te
		BlueAx | ..\Tex
		BloodSword | ..
		Sword | ..\Text
		Seed | ..\Textu
		ReturnFeather |*/

	return S_OK;
}

void CItemMgr::Release(void)
{
	for_each(m_MapItem.begin(), m_MapItem.end(), DeleteMap());
	m_MapItem.clear();
}

CPotion CItemMgr::Create_Potion(wstring wstrObjKey, int iCount)
{
	auto	iter = m_MapItem.find(wstrObjKey);
	
	iter->second->Get_ItemInfo().iCount = iCount;
	
	CPotion* pPotion = static_cast<CPotion*>(iter->second);
	return *pPotion;
}

CEquipment CItemMgr::Create_Equip(wstring wstrObjKey, int iCount)
{
	auto	iter = m_MapItem.find(wstrObjKey);

	iter->second->Get_ItemInfo().iCount = iCount;

	CEquipment* pEquipment = static_cast<CEquipment*>(iter->second);
	return *pEquipment;
}
