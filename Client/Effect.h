#pragma once
#include "Obj.h"
#include "Observer.h"

class CEffect :
	public CObj
{
public:
	CEffect();
	virtual ~CEffect();

	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual void Late_Progress(void) override;

protected:
	CObserver*			m_pObserver = nullptr;
	D3DXMATRIX			m_matParent;
};

