#pragma once

#include "Include.h"

class CObserver
{
public:
	virtual void	Update(int iFlag, void* pData)PURE;

public:
	CObserver();
	virtual ~CObserver();
};

