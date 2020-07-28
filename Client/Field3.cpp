#include "stdafx.h"
#include "Field3.h"

#include "ObjFactory.h"

CField3::CField3()
{
}


CField3::~CField3()
{
	Release();
}

HRESULT CField3::Initialize(void)
{
	CBack* pBack = CObjFactory<CBack>::CreateObj();
	pBack->Set_Stage(L"Field3");
	pBack->Camera_Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_BACK, pBack);

	wstring wstrStage = pBack->Get_wstrStage();
	int iIndex = pBack->Get_Player_Index(wstrStage);
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(iIndex);
	
	CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Boss.mp3");

	return S_OK;
}

int CField3::Progress(void)
{
	int iResult = 0;
	iResult = m_pObjMgr->Progress();

	if (CKeyMgr::Get_Instance()->KeyDown(KEY_1, 0))
	{
		CObjMgr::Get_Instance()->AddObject(OBJ_MONSTER, CObjFactory<CJelly>::CreateObj(332));
	}

	return 0;
}

void CField3::Render(void)
{
	m_pObjMgr->Render();
}

void CField3::Release(void)
{
	CObjMgr::Get_Instance()->DeleteObject(OBJ_BACK);
	CSoundMgr::Get_Instance()->StopAll();
}

void CField3::Late_Progress(void)
{
	m_pObjMgr->Late_Progress();
}
