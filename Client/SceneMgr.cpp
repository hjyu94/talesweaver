#include "stdafx.h"
#include "SceneMgr.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	: m_eSceneID(SCR_END)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}
void CSceneMgr::SetScene(SCENEID eID)
{
	if (nullptr != m_pScene)
		Safe_Delete(m_pScene);

	switch (eID)
	{
	case SC_LOGO:
		m_pScene = new CLogo;
		break;

	case SC_MENU:
		m_pScene = new CMainMenu;
		break;

	case SC_TOWN1:
		m_pScene = new CTown1;
		break;

	case SC_STORE0:
		m_pScene = new CStore0;
		break;

	case SC_STORE1:
		m_pScene = new CStore1;
		break;

	case SC_STORE2:
		m_pScene = new CStore2;
		break;

	case SC_FIELD0:
		m_pScene = new CField0;
		break;

	case SC_FIELD3:
		m_pScene = new CField3;
		break;
	}

	m_eSceneID = eID;
	m_pScene->Initialize();
}

int CSceneMgr::Progress(void)
{
	int iResult = 0;

	if (nullptr != m_pScene)
		iResult = m_pScene->Progress();

	return iResult;
}

void CSceneMgr::Late_Progress(void)
{
	m_pScene->Late_Progress();
}

void CSceneMgr::Render(void)
{
	if (nullptr != m_pScene)
		 m_pScene->Render();
}

void CSceneMgr::Release(void)
{
	Safe_Delete(m_pScene);
}