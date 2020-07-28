#pragma once
#include "Include.h"

class CMapMgr
{
	DECLARE_SINGLETON(CMapMgr)

public:
	CMapMgr();
	virtual ~CMapMgr();

public:
	HRESULT						Initialize(void);
	void						Release(void);
	map<wstring, BACK_INFO*>*	Get_BackInfoPtr() 
								{
									return &m_mapBackInfo;
								}

private:
	//map<wstring, CItem*>		m_MapItem;
	map<wstring, BACK_INFO*>	m_mapBackInfo;

};

