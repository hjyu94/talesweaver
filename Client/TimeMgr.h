#pragma once

#include "Include.h"

class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;

	float				m_fTime;

public:
	float		GetTime(void) { return m_fTime; }
	void		Initialize(void);
	void		SetTime(void);

private:
	CTimeMgr();
	~CTimeMgr();
};

