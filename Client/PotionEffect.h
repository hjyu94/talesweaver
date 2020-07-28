#pragma once
#include "Effect.h"
class CPotionEffect :
	public CEffect
{
public:
	CPotionEffect();
	CPotionEffect(POTIONID eID);
	virtual ~CPotionEffect();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Update_World_Matrix(void);

private:
	POTIONID	m_ePotionID;
};

