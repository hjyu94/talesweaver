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
	CObjMgr::Get_Instance()->AddObject(OBJ_UI, CObjFactory<CHPBar>::CreateObj());
	CObjMgr::Get_Instance()->AddObject(OBJ_UI, CObjFactory<CMPBar>::CreateObj());
	CObjMgr::Get_Instance()->AddObject(OBJ_UI, CObjFactory<CGauge>::CreateObj());
	
	CObj* pMenuUI = CObjFactory<CMenuUI>::CreateObj();
	CObjMgr::Get_Instance()->Set_MenuUI(pMenuUI);
	CObjMgr::Get_Instance()->AddObject(OBJ_UI, pMenuUI);

	CObjMgr::Get_Instance()->AddObject(OBJ_UI, CObjFactory<CQuickSlot>::CreateObj());
	CObjMgr::Get_Instance()->AddObject(OBJ_UI, CObjFactory<CChat>::CreateObj());
	CObjMgr::Get_Instance()->AddObject(OBJ_PLAYER, CObjFactory<CPlayer>::CreateObj());

	CObjMgr::Get_Instance()->AddObject(OBJ_BACK, CObjFactory<CBack>::CreateObj());


	// 확인
	CBack* pBack = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background());
	wstring wstrStage = pBack->Get_wstrStage();
	int iIndex = pBack->Get_Player_Index(wstrStage);
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(iIndex);

	
	// 아이템 바닥에 떨궈놓기
	CItem* pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP30"));
	pItem->Set_Pos(370);
	pBack->Push_Item(pItem);
	//CObjMgr::Get_Instance()->AddObject(OBJ_ITEM, pItem);

	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP80"));
	pItem->Set_Pos(371);
	pBack->Push_Item(pItem);
	//CObjMgr::Get_Instance()->AddObject(OBJ_ITEM, pItem);

	CNpc* pNPC = CObjFactory<CNpc>::CreateObj(369);

	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP10"));
	pNPC->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP30"));
	pNPC->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP50"));
	pNPC->Add_Item(pItem);
	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP80"));
	pNPC->Add_Item(pItem);

	CObjMgr::Get_Instance()->AddObject(OBJ_NPC, pNPC);

	// Gate
	CObjMgr::Get_Instance()->AddObject(OBJ_ETC, CObjFactory<CGate>::CreateObj(870));

	// Monster
	CObjMgr::Get_Instance()->AddObject(OBJ_MONSTER, CObjFactory<CJelly>::CreateObj(332));
	CObjMgr::Get_Instance()->AddObject(OBJ_MONSTER, CObjFactory<CBlueWolf>::CreateObj(334));

	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Town.wav");

	return S_OK;
}

int CStore0::Progress(void)
{
	int iResult = 0;
	iResult = m_pObjMgr->Progress();

	if (CKeyMgr::Get_Instance()->KeyDown(KEY_1, 0))
	{
		CObjMgr::Get_Instance()->AddObject(OBJ_MONSTER, CObjFactory<CJelly>::CreateObj(332));
	}

	// 몬스터~플레이어 충돌
	CCollisionMgr::CollisionRect(OBJ_PLAYER, OBJ_MONSTER);
	return iResult;
}

void CStore0::Render(void)
{
	m_pObjMgr->Render();
}

void CStore0::Release(void)
{
	
}

void CStore0::Late_Progress(void)
{
	m_pObjMgr->Late_Progress();
}
