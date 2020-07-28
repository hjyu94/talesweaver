#pragma once
#include "Effect.h"

// 마지막 스프라이트에서 정지 후 시간이 지나면 사라진다.
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

