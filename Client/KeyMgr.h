#pragma once

#include "Include.h"

class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)

public:
	enum STATE
	{
		S_DEFAULT, S_PLAYER, S_NPC, S_MONSTER, S_ITEM, S_UI_MENU, S_UI_QUICK
		, S_STATUS_WND, S_EQUIP_WND, S_INVEN_WND, S_SKILL_WND, S_SHOP_WND, S_END
	};

private:
	DWORD		m_dwKey;
	DWORD		m_dwPressed[S_END];
	DWORD		m_dwKeyDown[S_END];

public:
	void		SetKey(void);
	
public:
	bool		KeyPressed(DWORD dwKey, int iIndex);
	bool		KeyDown(DWORD dwKey, int iIndex);
	bool		KeyCombine(DWORD dwFirst, DWORD dwSecond, int iIndex = 0);

public:
	bool		KeyPressing(DWORD dwKey);

private:
	CKeyMgr();
	~CKeyMgr();
};

