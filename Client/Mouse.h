#pragma once
#include "Obj.h"
class CItem;

class CMouse :
	public CObj
{
public:
	CMouse();
	virtual ~CMouse();

public:
	enum STATE {
		S_DEFAULT, S_DEFAULT_CLICK
		, S_X, S_X_CLICK
		, S_ATTACK, S_ATTACK_CLICK
		, S_CATCH, S_CATCH_CLICK
		, S_DELAY, S_DELAY_CLICK
		, S_CHAT
		, S_GATE
		, S_MAGIC
		, S_END
	};

	enum PICK_MODE
	{
		PM_DEFAULT, PM_ITEM
	};

	enum ON { ON_ITEM, ON_MONSTER, ON_NPC, ON_UI, ON_SKILL, ON_END};
	// ON_UI: UI 위에 있는 상태에선 바닥을 클릭해도 가지 않는다.

public:
	// Inherited via CObj
	virtual HRESULT Initialize(void) override;
	virtual int		Progress(void) override;
	virtual void	Render(void) override;
	virtual void	Release(void) override;
	virtual void	Move_Frame(void) override;
	virtual void	Late_Progress(void) override;

public:
	void			Key_Check();
	virtual void	Update_World_Matrix(void);
	void			OnEffect(void);
	virtual void	Update_State(void);
	void			Move();
	void			Back_Default();
	void			Is_OnBtn_InLogo(void);
	bool			Is_On_UI(void) { return m_bIsOn[ON_UI]; }
	void			Cancel_Skill(void);

public: // 마우스 관련 함수
	void			Reset_On_Array(void);
	void			Pick(CItem* pItem);
	bool			Is_Picking_Sth(void) { return (m_ePickMode == PM_ITEM); }
	CItem*			Get_Picking_Item(void) { return m_pPickedItem; }
	void			End_Picking(void);

public: // Get
	STATE			Get_State(void) { return m_eCurState; }

public: // Set
	void			Set_State(ON eOn, bool bState) { m_bIsOn[eOn] = bState; }
	void			Set_State(STATE eState) { m_eNextState = eState; }

public:
	static	bool m_bIsOnLogoBtn;
	
private:
	PICK_MODE		m_ePickMode;
	CItem*			m_pPickedItem;

	STATE			m_eCurState;
	STATE			m_eNextState;

	int				m_iTile_Idx;
	TILE_DRAW		m_eDraw;
	TILE_OPTION		m_eOption;

	bool			m_bIsOn[ON_END];
};

