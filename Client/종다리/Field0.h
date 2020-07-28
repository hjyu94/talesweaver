#pragma once
#include "Scene.h"
class CField0 :
	public CScene
{
public:
	CField0();
	virtual ~CField0();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Late_Progress(void) override;
};

