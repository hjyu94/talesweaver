#pragma once

#include "Include.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"

class CScene
{
public:
	virtual HRESULT		Initialize(void)PURE;
	virtual int			Progress(void)PURE;
	virtual void		Render(void)PURE;
	virtual void		Release(void)PURE;
	virtual void		Late_Progress(void)PURE;

protected:
	CObjMgr*			m_pObjMgr;

public:
	CScene();
	virtual ~CScene();
};

