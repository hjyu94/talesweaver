#pragma once
#include "UI.h"
class CMonsterHPBar :
	public CUI
{
public:
	CMonsterHPBar();
	virtual ~CMonsterHPBar();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);

private:
	int			m_iHP = 0;
	int			m_iMaxHP = 0;
};

