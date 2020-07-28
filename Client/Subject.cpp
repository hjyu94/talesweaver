#include "stdafx.h"
#include "Subject.h"



CSubject::CSubject()
{
}


CSubject::~CSubject()
{
	Release();
}

void CSubject::Subscribe(CObserver* pObserver)
{
	m_ObserverList.push_back(pObserver);
}

void CSubject::UnSubscribe(CObserver* pObserver)
{
	for (auto iter = m_ObserverList.begin(); 
			iter != m_ObserverList.end(); )
	{
		if ((*iter) == pObserver)
		{
			iter = m_ObserverList.erase(iter);
		}
		else
			++iter;
	}
}

void CSubject::Notify(int iFlag, void* pData /*= NULL*/)
{
	for (auto& iter : m_ObserverList)
		iter->Update(iFlag, pData);
}

void CSubject::Release(void)
{
	m_ObserverList.clear();
}