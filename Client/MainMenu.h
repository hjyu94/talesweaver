#pragma once
#include "Scene.h"
#include "Mouse.h"

class CMainMenu :
	public CScene
{
public:
	CMainMenu();
	virtual ~CMainMenu();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Late_Progress(void) override;

public:
	CRITICAL_SECTION	Get_Crt(void) { return m_Crt; }
	bool				Is_Load_Over(void) { return (m_fLoadProgress >= 1.f); }

public:
	static size_t __stdcall Img_Load_Thread(void* pArg);

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	CMouse*				m_pMouse;
	CMyButton*			m_pStartBtn;
	CMyButton*			m_pExitBtn;

public:
	static int	m_iTotalLine;
	static int	m_iReadLine;
	float		m_fLoadProgress;

	static bool	m_bLoadEnd;
	bool		m_bBtnInit = false;


	int		m_iCount;
	float	m_fTime;
	bool	m_bMovieEnd;

};

