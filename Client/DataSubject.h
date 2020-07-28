#pragma once
#include "Subject.h"
class CDataSubject :
	public CSubject
{
	DECLARE_SINGLETON(CDataSubject)

public:
	list<void*>*	GetDataList(int iFlag);

public:
	void		AddData(int iFlag, void* pData);
	void		RemoveData(int iFlag, void* pData);
	void		Release(void);

private:
	map<int, list<void*>>			m_MapData;

public:
	CDataSubject();
	virtual ~CDataSubject();
};

