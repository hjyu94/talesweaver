#include "stdafx.h"
#include "Field0.h"

#include "ObjFactory.h"

CField0::CField0()
{
}


CField0::~CField0()
{
	Release();
}

HRESULT CField0::Initialize(void)
{
	CBack* pBack = CObjFactory<CBack>::CreateObj();
	pBack->Set_Stage(L"Field0");
	pBack->Camera_Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_BACK, pBack);

	// È®ÀÎ
	wstring wstrStage = pBack->Get_wstrStage();
	int iIndex = pBack->Get_Player_Index(wstrStage);
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(iIndex);

	// Monster
	int arrPosIndex[] = {
		465,
		645,
		674,
		847,
		934,
		958,
		994,
		1033,
		1077,
		1199,
		1283,
		1312,
		1351,
		1393,
		1411,
		1519,
		1618,
		1765,
		1768,
		1865
	};

	for (int i = 0; i < 20; ++i)
	{
		if(i%2 ==0)
			CObjMgr::Get_Instance()->AddObject(OBJ_MONSTER, CObjFactory<CJelly>::CreateObj(arrPosIndex[i]));
		else
			CObjMgr::Get_Instance()->AddObject(OBJ_MONSTER, CObjFactory<CBlueWolf>::CreateObj(arrPosIndex[i]));
	}

	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (!pPlayer->Is_Dead())
		CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Field.wav");
	else
		CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Boss.mp3");
	
	return S_OK;
}

int CField0::Progress(void)
{
	int iResult = 0;
	iResult = m_pObjMgr->Progress();

	return 0;
}

void CField0::Render(void)
{
	m_pObjMgr->Render();
}

void CField0::Release(void)
{
	CObjMgr::Get_Instance()->DeleteObject(OBJ_BACK);
	CObjMgr::Get_Instance()->DeleteObject(OBJ_MONSTER);
	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	CSoundMgr::Get_Instance()->StopAll();
}

void CField0::Late_Progress(void)
{
	m_pObjMgr->Late_Progress();
}
