#include "stdafx.h"
#include "DataSubject.h"

IMPLEMENT_SINGLETON(CDataSubject)

CDataSubject::CDataSubject()
{
}


CDataSubject::~CDataSubject()
{
	Release();
}


list<void*>* CDataSubject::GetDataList(int iFlag)
{
	auto	iter = m_MapData.find(iFlag);

	if (iter == m_MapData.end())
		return nullptr;

	return &iter->second;
}

void CDataSubject::AddData(int iFlag, void * pData)
{
	if (pData)
	{
		auto	iter = m_MapData.find(iFlag);

		if (iter == m_MapData.end())
		{
			m_MapData[iFlag] = list<void*>();
		}

		m_MapData[iFlag].push_back(pData);
		Notify(iFlag, pData);
	}

}

void CDataSubject::RemoveData(int iFlag, void * pData)
{
	auto	iter = m_MapData.find(iFlag);

	if (iter != m_MapData.end())
	{
		for (auto ListIter = m_MapData[iFlag].begin();
			ListIter != m_MapData[iFlag].end();)
		{
			if ((*ListIter) == pData)
			{
				ListIter = m_MapData[iFlag].erase(ListIter);
			}
			else
				ListIter++;
		}
	}


}

void CDataSubject::Release(void)
{
	for (auto& iter : m_MapData)
		iter.second.clear();

	m_MapData.clear();
}
