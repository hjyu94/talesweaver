#pragma once
#include "Scene.h"
class CField3 :
	public CScene
{
public:
	CField3();
	virtual ~CField3();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Late_Progress(void) override;
};

