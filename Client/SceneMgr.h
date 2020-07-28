#pragma once

#include "Logo.h"
#include "MainMenu.h"
#include "Town1.h"
#include "Field0.h"
#include "Field3.h"
#include "Store0.h"
#include "Store1.h"
#include "Store2.h"

class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	CScene*		m_pScene = nullptr;
	SCENEID		m_eSceneID;
public:
	void		SetScene(SCENEID eID);
	int			Progress(void);
	void		Late_Progress(void);
	void		Render(void);
	void		Release(void);

public:
	SCENEID		Get_SceneID(void) { return m_eSceneID; }

private:
	CSceneMgr();
	~CSceneMgr();
};

