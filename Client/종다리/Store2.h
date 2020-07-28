#pragma once
#include "Scene.h"
class CStore2 :
	public CScene
{
public:
	CStore2();
	virtual ~CStore2();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Late_Progress(void) override;
};

