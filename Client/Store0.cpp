#include "stdafx.h"
#include "Store0.h"
#include "ObjFactory.h"

CStore0::CStore0()
{
}


CStore0::~CStore0()
{
	Release();
}

HRESULT CStore0::Initialize(void)
{
	CBack* pBack = CObjFactory<CBack>::CreateObj();
	pBack->Set_Stage(L"Store0");
	pBack->Camera_Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_BACK, pBack);

	// È®ÀÎ
	wstring wstrStage = pBack->Get_wstrStage();
	int iIndex = pBack->Get_Player_Index(wstrStage);
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(iIndex);

	CNpc* pNpc = new CNpc(L"Linda");
	pNpc->Set_Pos(333);
	pNpc->Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_NPC, pNpc);
	
	CItem* pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"LeatherShoes"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"RedShoes"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"RedFoot"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"GreenFoot"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"RedRing"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"GreenRing"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"BloodSwordSingleTex"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"LightSword"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"MagicWand"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"Mace"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"Sword"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"BlueAx"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"DevilSword"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"HeavyArmor"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"LightArmor"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"FabricHood"));
	pNpc->Add_Item(pItem);
	pItem = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"ClradHat"));
	pNpc->Add_Item(pItem);

	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (!pPlayer->Is_Dead())
		CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Store.wav");
	else
		CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Boss.mp3");


	return S_OK;
}

int CStore0::Progress(void)
{
	int iResult = 0;
	iResult = m_pObjMgr->Progress();
	return iResult;
}

void CStore0::Render(void)
{
	m_pObjMgr->Render();
}

void CStore0::Release(void)
{
	CObjMgr::Get_Instance()->DeleteObject(OBJ_BACK);
	CObjMgr::Get_Instance()->DeleteObject(OBJ_NPC);
	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	CSoundMgr::Get_Instance()->StopAll();
}

void CStore0::Late_Progress(void)
{
	m_pObjMgr->Late_Progress();
}
