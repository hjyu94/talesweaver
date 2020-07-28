#pragma once
#include "WndUI.h"
class CNpc;
class CEventWnd :
	public CWndUI
{
public:
	CEventWnd();
	virtual ~CEventWnd();

public:
	// CWndUI을(를) 통해 상속됨
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	void		Quest_Get(void);
	void		Up_MonsterKill(void);
	bool		Is_Cleared_Quest(void) { return m_bQuestSuccess; }

public:
	bool		m_bQuestStart = false;
	bool		m_bQuestSuccess = false;
	int			m_iQuestProcess = 0;
	int			m_iMonster = 0;
	wstring		wstrQuest;
};

