#include "stdafx.h"
#include "MapMgr.h"

IMPLEMENT_SINGLETON(CMapMgr)

CMapMgr::CMapMgr()
{
}


CMapMgr::~CMapMgr()
{
	Release();
}

HRESULT CMapMgr::Initialize(void)
{
	wstring wstrStage = L"Town1";
	BACK_INFO* pBack = new BACK_INFO;
	pBack->ePlayerDir = D_LD;
	pBack->iPlayerIndex = 531;
	m_mapBackInfo.emplace(wstrStage, pBack);

	wstrStage = L"Store0";
	pBack = new BACK_INFO;
	pBack->ePlayerDir = D_LU;
	pBack->iPlayerIndex = 471;
	m_mapBackInfo.emplace(wstrStage, pBack);

	wstrStage = L"Store1";
	pBack = new BACK_INFO;
	pBack->ePlayerDir = D_LU;
	pBack->iPlayerIndex = 307;
	m_mapBackInfo.emplace(wstrStage, pBack);

	wstrStage = L"Store2";
	pBack = new BACK_INFO;
	pBack->ePlayerDir = D_LU;
	pBack->iPlayerIndex = 414;
	m_mapBackInfo.emplace(wstrStage, pBack);

	wstrStage = L"Field0";
	pBack = new BACK_INFO;
	pBack->ePlayerDir = D_RD;
	pBack->iPlayerIndex = 1348;
	m_mapBackInfo.emplace(wstrStage, pBack);

	wstrStage = L"Field3";
	pBack = new BACK_INFO;
	pBack->ePlayerDir = D_RD;
	pBack->iPlayerIndex = 2266;
	m_mapBackInfo.emplace(wstrStage, pBack);


	wstrStage = L"Town0";
	pBack = new BACK_INFO;
	pBack->ePlayerDir = D_RD;
	pBack->iPlayerIndex = 100;
	m_mapBackInfo.emplace(wstrStage, pBack);

	return S_OK;
}

void CMapMgr::Release(void)
{
	for (auto& iter = m_mapBackInfo.begin(); iter != m_mapBackInfo.end(); )
	{
		if (iter->second)
		{
			delete iter->second;
			iter->second = nullptr;
			iter = m_mapBackInfo.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	m_mapBackInfo.clear();
}
