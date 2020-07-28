#pragma once
#include "Monster.h"

class CHPBar;

class CJelly :
	public CMonster
{
public:
	CJelly();
	virtual ~CJelly();

public:
	enum STATE
	{
		S_ATTACK, S_DAMAGE, S_DIE, S_STAND, S_WALK, S_END
	};


public:
	virtual HRESULT Initialize(void) override;
	virtual int		Progress(void) override;
	virtual void	Render(void) override;
	virtual void	Release(void) override;
	virtual void	Update_State(void);
	virtual	void	Move_Frame(void);

public:
	// CMonster��(��) ���� ��ӵ�
	virtual void	Before_Update_State(void) override;
	
private:
	STATE			m_eCurState;
	STATE			m_eNextState;

	float			m_fDamageRenderTime;
};

