#pragma once
#include "Monster.h"
class CBlueWolf :
	public CMonster
{
public:
	CBlueWolf();
	virtual ~CBlueWolf();

public:
	enum STATE {
		S_ATTACK, S_DAMAGE, S_STAND, S_WALK, S_END
	};


public:
	virtual HRESULT Initialize(void) override;
	virtual int		Progress(void) override;
	virtual void	Render(void) override;
	virtual void	Release(void) override;
	virtual void	Update_State(void);
	virtual	void	Move_Frame(void);

public:
	// CMonster을(를) 통해 상속됨
	virtual void	Before_Update_State(void) override;

private:
	STATE			m_eCurState;
	STATE			m_eNextState;
};


