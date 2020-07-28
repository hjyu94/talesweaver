#pragma once

//#include "BackGround.h"
#include "Obj.h"
#include "Back.h"
#include "Mouse.h"
#include "MyButton.h"
#include "Player.h"
#include "HPBar.h"
#include "MPBar.h"
#include "Gauge.h"
#include "MenuUI.h"
#include "QuickSlot.h"
#include "Chat.h"

#include "MenuUI.h"
#include "EquipWnd.h"
#include "InvenWnd.h"
#include "SkillWnd.h"
#include "StatusWnd.h"
#include "EventWnd.h"

#include "Npc.h"
#include "Item.h"
#include "Gate.h"

#include "Damage.h"
#include "Weapon.h"
#include "SkillIcon.h"

#include "ItemMgr.h"

#include "PotionEffect.h"
#include "EffectBuff.h"
#include "EffectExtinction.h"

#include "Jelly.h"
#include "BlueWolf.h"
#include "Etc.h"

#include "CoolEffect.h"
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)

public:
	void			AddObject(OBJID eID, CObj* pObj);
	HRESULT			Initialize(void);
	int				Progress(void);
	void			Late_Progress(void);
	void			Render(void);
	void			Release(void);
	
public:
	void			DeleteObject(OBJID eID);


public:
	CObj*					Get_Background(void);
	CObj*					Get_Player(void);
	CObj*					Get_Mouse(void);
	CMouse::STATE			Get_Mouse_State(void);
	list<CObj*>*			Get_ObjListPtr(OBJID eID);
	CObj*					Get_MenuUI(void) { return m_pMenuUI; }
	CObj*					Get_ChatUI(void) { return m_pChatUI; }

public:
	void					Set_MenuUI(CObj* pObj) { m_pMenuUI = pObj; }
	void					Set_ChatUI(CObj* pObj) { m_pChatUI = pObj; }

private:
	list<CObj*>		m_ObjList[OBJ_END];
	list<CObj*>		m_RenderSort[RDR_END];

	CObj*			m_pMenuUI = nullptr;
	CObj*			m_pChatUI = nullptr;
	
private:
	CObjMgr();
	~CObjMgr();
};

