#pragma once
#include "Obj.h"
#include "BattleUnit.h"
class CMonster : public CBattleUnit
{
public:
	CMonster();
	virtual ~CMonster();

public:
	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Update_World_Matrix(void);

public:
	void			IsNearPlayer(void);
	void			Move(void);
	virtual void	Before_Update_State(void) PURE;
	void			Update_Direction(void);
	void			IsAttacking(void);
	virtual void	Get_Damage(CBattleUnit* pAttacker) override;
	void			Attack(int iDamage);
	
	int				Get_ReturnExp(void) { return m_iReturnExp; }
	wstring			Get_ReturnItem(void) { return m_wstrReturnItem; }

	void			Set_State(BOOL eState, bool bTrue) { m_bState[eState] = bTrue; }
	
public:
	enum BOOL
	{
		B_NEAR_PLAYER, B_DAMAGED, B_WALK, B_ATTACK, B_END
	};

protected:
	float		m_fDirSetTime;
	float		m_fMoveTime;
	float		m_fAttackTime;
	float		m_fDamageTime;

	wstring		m_wstrDir;
	float		m_fSpeed;

	DIR			m_eCurDir;
	DIR			m_eNextDir;

	int			m_iTileIndex;

	int			m_iAtt;
	int			m_iDef;
	int			m_iReturnExp;
	wstring		m_wstrReturnItem;

	bool		m_bState[B_END];
	D3DXVECTOR3	m_vNutBackDir;
};

