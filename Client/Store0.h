#pragma once
#include "Scene.h"

class CStore0 :
	public CScene
{
public:
	CStore0();
	virtual ~CStore0();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Late_Progress(void) override;
};

