#pragma once
#include "Effect.h"

// ��������Ʈ�� �ѹ� �� ���� �������.
class CCoolEffect :
	public CEffect
{
public:
	CCoolEffect();
	virtual ~CCoolEffect();

public:
	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
};


