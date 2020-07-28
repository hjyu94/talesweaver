#pragma once
#include "Obj.h"
#include "Observer.h"
class CPlayer;

class CWeapon :
	public CObj
{
public:
	CWeapon();
	CWeapon(wstring wstrObjKey);
	virtual ~CWeapon();

public:
	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int		Progress(void) override;
	virtual void	Render(void) override;
	virtual void	Release(void) override;
	virtual void	Update_World_Matrix(void);
	virtual void	Update_State(void);

	void			Update_Direction(void);
	virtual void	Late_Progress(void) override;

public:
	enum STATE
	{
		S_HORCUT, S_STAB, S_STAND, S_VERCUT, S_END
	};

public:
	void		Set_Show(bool bShow) { m_bShow = bShow; }
	void		Set_Owner(CPlayer* pPlayer) { m_pOwner = pPlayer; }

private:
	bool		m_bShow;

	wstring		m_wstrDir;
	
	DIR			m_eCurDir;
	DIR			m_eNextDir;

	STATE		m_eCurState;
	STATE		m_eNextState;

	CObserver*	m_pObserver = nullptr;
	CPlayer*	m_pOwner = nullptr;
};

