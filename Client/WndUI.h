#pragma once
#include "UI.h"
class CMenuUI;

class CWndUI :
	public CUI
{
public:
	CWndUI();
	virtual ~CWndUI();

public:
	void	Show(bool bIsShow) { m_bIsShow = bIsShow; }

public:
	RECT	Get_TitleRect(void) { return m_rcTitle; }
	int		Get_Priority(void) { return m_iPriority; }

public:
	void	Set_Menu(CMenuUI* pMenu) { m_pMenu = pMenu; }
	void	Set_Priority(int iPriority) { m_iPriority = iPriority; }
	
public:
	void	IsShopping(bool bShopping) { m_bIsShopping = bShopping; }

protected:
	bool		m_bIsShow = false;
	int			m_iPriority;
	bool		m_bIsShopping = false;

	D3DXVECTOR3 m_vExitBtnPos;
	RECT	m_rcExitBtn;
	bool	m_bExitBtnOn;

	RECT	m_rcTitle;
	float	m_fWidth;
	float	m_fHeight;

	bool		m_bDragging = false;
	D3DXVECTOR3 m_vDragDelta;

	D3DXVECTOR3 m_vOriginalPos;
	CMenuUI*	m_pMenu;
public:
	// CUI을(를) 통해 상속됨
	virtual int Progress(void) override;
	virtual void Render(void) override;
};

