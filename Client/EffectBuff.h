#pragma once
#include "Effect.h"

// ������ ��������Ʈ���� ���� �� �ð��� ������ �������.
class CEffectBuff :
	public CEffect
{
public:
	CEffectBuff();
	CEffectBuff(wstring wstrObjKey, wstring wstrStateKey, float fMaxTime);
	virtual ~CEffectBuff();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	float		m_fMaxTime;
	float		m_fCurTime;
};

