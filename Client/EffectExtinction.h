#pragma once
#include "Effect.h"

// ��������Ʈ�� �ѹ� �� ���� �������.
class CEffectExtinction :
	public CEffect
{
public:
	CEffectExtinction();
	CEffectExtinction(wstring wstrObjKey, wstring wstrStateKey);
	virtual ~CEffectExtinction();

public:
	// CObj��(��) ���� ��ӵ�
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
};


