#pragma once
#include "Obj.h"
class CDamage;
class CBattleUnit : public CObj
{
public:
	CBattleUnit();
	virtual ~CBattleUnit();

public:
	STATUS_INFO&	Get_Status(void) { return m_tStatus; }

public:
	virtual void	Release(void) override;
	bool			Is_Shield(void) { return m_bShield; }
	bool			Is_Dead(void) { return m_bIsDead; }
	virtual void	Get_Damage(CBattleUnit* pAttacker) PURE;
	virtual void	Late_Progress(void) override;
	void			Set_Dead(bool bTrue) { m_bIsDead = bTrue; }

protected:
	STATUS_INFO		m_tStatus;
	bool			m_bIsDead = false;
	bool			m_bShield = false;
};

