#pragma once

#include "Observer.h"

class CSubject
{
protected:
	list<CObserver*>			m_ObserverList;

public:
	void		Subscribe(CObserver* pObserver);
	void		UnSubscribe(CObserver* pObserver);

	void		Notify(int iFlag, void* pData = NULL);
	void		Release(void);

public:
	CSubject();
	virtual ~CSubject();
};

