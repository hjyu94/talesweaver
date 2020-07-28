#include "stdafx.h"
#include "Store1.h"
#include "ObjFactory.h"

CStore1::CStore1()
{
}


CStore1::~CStore1()
{
	Release();
}

HRESULT CStore1::Initialize(void)
{
	CBack* pBack = CObjFactory<CBack>::CreateObj();
	pBack->Set_Stage(L"Store1");
	pBack->Camera_Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_BACK, pBack);

	wstring wstrStage = pBack->Get_wstrStage();
	int iIndex = pBack->Get_Player_Index(wstrStage);
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(iIndex);

	CNpc* pNpc = new CNpc(L"Kenishi");
	pNpc->Set_Pos(190);
	pNpc->Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_NPC, pNpc);

	CItem* pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP10"));
	pNpc->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP50"));
	pNpc->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP80"));
	pNpc->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP100"));
	pNpc->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"MP10"));
	pNpc->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"MP50"));
	pNpc->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"MP80"));
	pNpc->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"MP100"));
	pNpc->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"SP10"));
	pNpc->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"SP50"));
	pNpc->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"SP80"));
	pNpc->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"SP100"));
	pNpc->Add_Item(pItem);


	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (!pPlayer->Is_Dead())
		CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Store.wav");
	else
		CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Boss.mp3");

	return S_OK;
}

int CStore1::Progress(void)
{
	int iResult = 0;
	iResult = m_pObjMgr->Progress();
	return iResult;

	
}

void CStore1::Render(void)
{
	m_pObjMgr->Render();
}

void CStore1::Release(void)
{
	CObjMgr::Get_Instance()->DeleteObject(OBJ_BACK);
	CObjMgr::Get_Instance()->DeleteObject(OBJ_NPC);
	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	CSoundMgr::Get_Instance()->StopAll();

}

void CStore1::Late_Progress(void)
{
	m_pObjMgr->Late_Progress();
}
