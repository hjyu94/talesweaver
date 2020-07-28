#pragma once
#include "Obj.h"
#include "Observer.h"

class CUI :
	public CObj
{
public:
	CUI();
	virtual ~CUI();

protected:
	CObserver*			m_pObserver = nullptr;

public:
	virtual HRESULT Initialize(void) PURE;
	virtual int Progress(void) PURE;
	virtual void Render(void) PURE;
	virtual void Release(void) PURE;
	virtual void Late_Progress(void) override;
};

