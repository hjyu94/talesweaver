#pragma once
#include "Obj.h"
#include "Item.h"
#include "Equipment.h"
#include "Potion.h"

#include "BattleUnit.h"

class CWeapon;
class CSkill;

class CPlayer : public CBattleUnit
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	// Inherited via CObj
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual	void Move_Frame(void) override;
	
public:
	enum STATE {
		S_STAND, S_RUN, S_SIT, S_SKILL, S_WALK,
		S_HORCUT, S_STAB, S_VERCUT, S_DAMAGE, S_END
	};

	enum MOVE_STATE {
		MS_WALK, MS_RUN
	};

	enum ON {
		ON_SIT, ON_HORCUT, ON_VERCUT, ON_STAB, ON_DAMAGE, ON_OBJECT, ON_WALK, ON_END
	};

public:
	void			Key_Check();
	void			Move();
	void			Move(D3DXVECTOR3 vDestination);
	void			Is_On_Gate();
	void			OnEffect(void);

	void			Update_State();
	void			Update_Direction();
	void			Offset();
	void			Update_World_Matrix();
	
	void			Astar_Start();
	//int				Get_TileIndex(void) { return m_iCur_Idx; }
	
	void			Back_Default();
	void			IsRecovering();
	void			Set_Direction(DIR eDir) { m_eNextDir = eDir; }
	
	void			Set_State(ON eOn, bool bState) { m_bIsOn[eOn] = bState; }
	void			Reset_On_Array(void);


	// �κ��丮
	vector<CItem*>*	Get_InvenPtr() { return &m_vecInven; }
	CEquipment**	Get_EquipPtr() { return m_arrEquip; }
	void			Eat_item(CItem* pItem);
	void			Use_Item(CItem* pItem);
	void			UnEqiup_Item(CEquipment* pItem);
	void			Select_Skill(wstring wstrSkill);
	void			Cancel_Skill();
	void			Use_Skill();
	void			Set_Shield(bool bIsOn) { m_bShield = bIsOn; }

	// ����
	void			Buy_Item(CItem* pItem);
	void			Sell_Item(CItem* pItem);
	
	// ����
	void			Set_Target(CBattleUnit* pTarget) { m_pTarget = pTarget; }
	// CBattleUnit��(��) ���� ��ӵ�
	virtual void	Get_Damage(CBattleUnit * pAttacker) override;	
	PLAYER_INFO&	Get_PlayerStatus(void) { return m_tPlayerInfo; }
	void			Attack(CBattleUnit* pTarget, int iDamage);
	void			Level_Up(bool bOverMaxExp);
	void			Dead(void); // ����
	void			Revive(void); // ��Ȱ�ϱ�
	bool			Is_Powerful(void) { return m_bPowerful; }

	// ������Ʈ �浹
	void			Collision(bool bTrue) { m_bIsOn[ON_OBJECT] = bTrue; }

	// ���� ���� �ٲٱ�
	CPlayer::STATE	Get_CurState() { return m_eCurState; }

	// NPC ��ȣ�ۿ�
	void			Cure_From_Npc(void);

private:
	// �̵�
	float			m_fSpeed;

	STATE			m_eCurState;
	STATE			m_eNextState;

	DIR				m_eCurDir;
	DIR				m_eNextDir;
	std::wstring	m_wstrDir;

	int				m_iDst_Idx;
	int				m_iCur_Idx;
	list<int>		m_BestList;
	D3DXVECTOR3		m_vMoveDest;

	MOVE_STATE		m_eMove;

	// ����
	PLAYER_INFO		m_tPlayerInfo;

	// ���, �κ�
	vector<CItem*>	m_vecInven;
	CEquipment*		m_arrEquip[EQ_END];

	// ����
	D3DXMATRIX		m_matTrans;
	D3DXMATRIX		m_matScale;
	CBattleUnit*	m_pTarget = nullptr;
	CWeapon*		m_pWeapon;
	wstring			m_wstrSkill;
	bool			m_bNoPower = false;

	// On State
	bool			m_bIsOn[ON_END];
	float			m_fDamageTime;
	D3DXVECTOR3		m_vNutBackDir;
	
	bool			m_bPowerful = false;
};