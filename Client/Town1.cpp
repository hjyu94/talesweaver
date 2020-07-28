#include "stdafx.h"
#include "Town1.h"
#include "ObjFactory.h"

CTown1::CTown1()
{
}


CTown1::~CTown1()
{
	Release();
}

HRESULT CTown1::Initialize(void)
{
	CBack* pBack = CObjFactory<CBack>::CreateObj();
	pBack->Set_Stage(L"Town1");
	pBack->Camera_Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_BACK, pBack);

	wstring wstrStage = pBack->Get_wstrStage();
	int iIndex = pBack->Get_Player_Index(wstrStage);
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(iIndex);


	// 아이템 바닥에 떨궈놓기
	CItem* pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP30"));
	pItem->Set_Pos(296);
	pBack->Push_Item(pItem);

	pItem = new CPotion(CItemMgr::Get_Instance()->Create_Potion(L"HP80"));
	pItem->Set_Pos(297);
	pBack->Push_Item(pItem);


	CEquipment* pEquipment = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"LightSword"));
	pEquipment->Set_Pos(313);
	pBack->Push_Item(pEquipment);

	pEquipment = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"LightArmor"));
	pEquipment->Set_Pos(314);
	pBack->Push_Item(pEquipment);

	pEquipment = new CEquipment(CItemMgr::Get_Instance()->Create_Equip(L"BloodSwordSingleTex"));
	pEquipment->Set_Pos(315);
	pBack->Push_Item(pEquipment);

	// Gate
	CObjMgr::Get_Instance()->AddObject(OBJID::OBJ_EFFECT, CObjFactory<CGate>::CreateObj(780));

	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (!pPlayer->Is_Dead())
		CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Town.wav");
	else
		CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Boss.mp3");

	return S_OK;
}

int CTown1::Progress(void)
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

void CTown1::Render(void)
{
	m_pObjMgr->Render();
}

void CTown1::Release(void)
{
	CObjMgr::Get_Instance()->DeleteObject(OBJ_BACK);
	CObjMgr::Get_Instance()->DeleteObject(OBJ_MONSTER);
	CObjMgr::Get_Instance()->DeleteObject(OBJ_NPC);
	CObjMgr::Get_Instance()->DeleteObject(OBJ_ETC);

	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	CSoundMgr::Get_Instance()->StopAll();
}

void CTown1::Late_Progress(void)
{
	m_pObjMgr->Late_Progress();
}
