#pragma once
#include "Scene.h"
class CStore1 :
	public CScene
{
public:
	CStore1();
	virtual ~CStore1();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Late_Progress(void) override;

};

