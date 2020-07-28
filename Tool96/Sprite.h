#pragma once

#include "Include.h"

class CTool96View;
class CSpriteTab;

class CSprite
{
public:
	CSprite();
	~CSprite();


public:
	HRESULT				Initialize(void);
	int					Progress(void);
	void				Render(void);
	void				Release(void);

public: // Set
	void				SetMainView(CTool96View* pView) { m_pMainView = pView; }
	void				Set_pTexture(const TEXINFO*	pTexture) { m_pTexture = pTexture; }
	void				Set_Mid_Point(POINT point);

public: // Get
	CSpriteTab*			Get_pSpriteTab(void) { return m_pSpriteTab; }
	POINT				Get_Mid_Point() { return POINT{ m_iMiddleX, m_iMiddleY }; }
	
private:
	CTool96View*		m_pMainView = nullptr;
	CSpriteTab*			m_pSpriteTab = nullptr;
	const TEXINFO*		m_pTexture = nullptr;

	int					m_iMiddleX = 0;
	int					m_iMiddleY = 0;

};

