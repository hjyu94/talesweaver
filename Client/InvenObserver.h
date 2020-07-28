#pragma once
#include "Observer.h"
#include "Item.h"

class CInvenObserver :
	public CObserver
{
public:
	CInvenObserver();
	virtual ~CInvenObserver();

public:
	const vector<CItem*>* Get_PlayerInven(void) { 
		return &m_vecPlayerInven;
	}

public:
	// CObserver��(��) ���� ��ӵ�
	virtual void Update(int iFlag, void * pData) override;

private:
	vector<CItem*>			m_vecPlayerInven;

};

