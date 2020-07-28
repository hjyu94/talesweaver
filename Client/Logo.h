#pragma once
#include "Scene.h"
class CLogo :
	public CScene
{
public:
	CLogo();
	virtual ~CLogo();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Late_Progress(void) override;

private:
	float	m_fTime;
	int		m_iCount;
	bool	m_bMovieEnd;

	bool	m_bInit = false;
};

