#pragma once
#include "Scene.h"

class CTown1 : public CScene
{
public:
	CTown1();
	virtual ~CTown1();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Late_Progress(void) override;
};

