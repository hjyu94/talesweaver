#pragma once
#include "UI.h"
class CDamage :
	public CUI
{
public:
	CDamage();
	CDamage(int iDamage);
	virtual ~CDamage();

public:
	// CUI을(를) 통해 상속됨
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	void	Critical(bool bCritical) { m_bCritical = bCritical; }

private:
	int		m_iDamage;
	int		m_iCount;
	bool	m_bCritical;

	float	m_fTime;
	float	m_fScale;

	int		m_iProgress = 1;
};

