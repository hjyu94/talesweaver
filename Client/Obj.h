#pragma once

#include "TimeMgr.h"
#include "TextureMgr.h"
#include "GraphicDev.h"
#include "Astar.h"
#include "Include.h"

class CDamage;

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	static D3DXVECTOR3		m_vScroll;

protected:
	INFO					m_tInfo;
	RECT					m_tRect;
	FRAME					m_tFrame;
	RENDERID				m_eRenderID = RDR_GAMEOBJECT;

public:
	virtual					HRESULT	Initialize(void)	PURE;
	virtual					int		Progress(void)		PURE;
	virtual					void	Late_Progress(void)	PURE;
	virtual					void	Render(void)		PURE;
	virtual					void	Release(void)		PURE;
	

public:
	virtual	void	Move_Frame(void)
	{
		m_tFrame.fFrameTime += GET_TIME;

		if (m_tFrame.fFrameTime > m_tFrame.fFrameSpeed)
		{
			m_tFrame.fFrameTime = 0;
			m_tFrame.iSpriteCnt++;
		}

		if (m_tFrame.iSpriteCnt >= m_tFrame.iSpriteMax)
		{
			m_tFrame.iSpriteCnt = 0;
		}
	}

	void			Set_Pos(int iTileIdx);
	void			Set_Pos(D3DXVECTOR3 vPos);

public: // Get func
	INFO			Get_Info(void) { return m_tInfo; }
	RECT			Get_Rect(void) { return m_tRect; }
	FRAME			Get_Frame(void) { return m_tFrame; }
	RENDERID		Get_RenderID(void) { return m_eRenderID; }
};