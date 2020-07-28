#pragma once

#include "Include.h"
#include "GraphicDev.h"
#include "SceneMgr.h"
#include "TimeMgr.h"
#include "ItemMgr.h"

class CMainGame
{
private:
	CGraphicDev*		m_pGraphicDev;
	CSceneMgr*			m_pSceneMgr;
	CTimeMgr*			m_pTimeMgr;

	float				m_fTime;
	int					m_iCount;
	TCHAR				m_szFPS[MIN_STR];
	D3DXMATRIX			m_matTrans; // FPS 출력 위치

public:
	HRESULT		Initialize(void);
	void		Progress(void);
	void		Late_Progress(void);
	void		Render(void);
	void		Release(void);
	
	
public:
	CMainGame();
	~CMainGame();
};

