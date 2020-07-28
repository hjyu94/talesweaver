#include "stdafx.h"
#include "KeyMgr2.h"

CKeyMgr2* CKeyMgr2::m_pInstance = nullptr; 

CKeyMgr2::CKeyMgr2()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}


CKeyMgr2::~CKeyMgr2()
{
}

bool CKeyMgr2::KeyPressing(int iKey)
{
	if (GetAsyncKeyState(iKey) & 0x8000)
		return true;

	return false;
}

bool CKeyMgr2::KeyDown(int iKey)
{
	if (!m_bKeyState[iKey] && (GetAsyncKeyState(iKey) & 0x8000))
	{
		m_bKeyState[iKey] = !m_bKeyState[iKey];
		return true;
	}
	return false;
}

bool CKeyMgr2::KeyUP(int iKey)
{
	if (m_bKeyState[iKey] && !(GetAsyncKeyState(iKey) & 0x8000))
	{
		m_bKeyState[iKey] = !m_bKeyState[iKey];
		return true;
	}
	return false;
}

void CKeyMgr2::KeyUpdate()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
}
