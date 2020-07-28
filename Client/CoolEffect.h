#pragma once
#include "Effect.h"

// 스프라이트가 한번 쭉 돌면 사라진다.
class CCoolEffect :
	public CEffect
{
public:
	CCoolEffect();
	virtual ~CCoolEffect();

public:
	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
};


