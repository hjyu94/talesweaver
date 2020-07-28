#include "stdafx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
	: m_dwKey(0)
	//, m_dwPressed(0)
{
	ZeroMemory(m_dwKeyDown, sizeof(m_dwKeyDown));
}


CKeyMgr::~CKeyMgr()
{
}

// Maingame::Progress() ���� ȣ���Ѵ�
// ������ �ִ� Ű�� �������� ��Ʈ�����ڷ� ����Ѵ�.
void CKeyMgr::SetKey(void)
{
	m_dwKey = 0;
	
	// GetAsyncKeyState ���� ���� 8000 �̰ų� 8001 �̸� true
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_RETURN;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;

	if (GetAsyncKeyState('1') & 0x8000)
		m_dwKey |= KEY_1;

	if (GetAsyncKeyState('2') & 0x8000)
		m_dwKey |= KEY_2;

	if (GetAsyncKeyState('3') & 0x8000)
		m_dwKey |= KEY_3;

	if (GetAsyncKeyState('4') & 0x8000)
		m_dwKey |= KEY_4;

	if (GetAsyncKeyState('5') & 0x8000)
		m_dwKey |= KEY_5;

	if (GetAsyncKeyState('6') & 0x8000)
		m_dwKey |= KEY_6;

	if (GetAsyncKeyState('7') & 0x8000)
		m_dwKey |= KEY_7;

	if (GetAsyncKeyState('8') & 0x8000)
		m_dwKey |= KEY_8;

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_dwKey |= KEY_SHIFT;

	if (GetAsyncKeyState('R') & 0x8000)
		m_dwKey |= KEY_R;

	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_S;


	if (GetAsyncKeyState(VK_F1) & 0x8000)
		m_dwKey |= KEY_F1;

	if (GetAsyncKeyState(VK_F2) & 0x8000)
		m_dwKey |= KEY_F2;

	if (GetAsyncKeyState(VK_F3) & 0x8000)
		m_dwKey |= KEY_F3;

	if (GetAsyncKeyState(VK_F4) & 0x8000)
		m_dwKey |= KEY_F4;

	if (GetAsyncKeyState(VK_F5) & 0x8000)
		m_dwKey |= KEY_F5;

	if (GetAsyncKeyState(VK_F6) & 0x8000)
		m_dwKey |= KEY_F6;

	if (GetAsyncKeyState(VK_F7) & 0x8000)
		m_dwKey |= KEY_F7;

	if (GetAsyncKeyState(VK_F8) & 0x8000)
		m_dwKey |= KEY_F8;
}

bool CKeyMgr::KeyPressed(DWORD dwKey, int iIndex)
{
	// �ش� Ű��(dwKey) ���� ������(m_dwKey) ������ false
	// ������ �ִٰ� ǥ�����ش� (|= dwKey)
	if (m_dwKey & dwKey)
	{
		m_dwPressed[iIndex] |= dwKey;
		return false;
	}

	// �ش� Ű��(dwKey) ������ �־����� ����� ������ ���� ���(���� ���ǿ� �ɸ��� ����) true�� ��ȯ 
	// ���� ������ �ִ� Ű(m_dwKey)�� ������ ������ ǥ��
	else if (m_dwPressed[iIndex] & dwKey)
	{		
		m_dwPressed[iIndex] = m_dwKey;
		return true;
	}
	
	return false;
}						
							
bool CKeyMgr::KeyDown(DWORD dwKey, int iIndex)
{				
	if (KeyPressed(dwKey, iIndex))
		m_dwKeyDown[iIndex] ^= dwKey;
											
	if (!(m_dwKeyDown[iIndex] & dwKey))
	{			
		if (m_dwKey & dwKey)
		{
			m_dwKeyDown[iIndex] |= dwKey;
			return true;
		}
	}

	return false;
}

bool CKeyMgr::KeyCombine(DWORD dwFirst, DWORD dwSecond, int iIndex)
{
	if (m_dwKey & dwFirst)
	{
		if (KeyDown(dwSecond, iIndex))
			return true;
	}

	return false;
}

bool CKeyMgr::KeyPressing(DWORD dwKey)
{
	if (GetAsyncKeyState(dwKey) & 0x8000)
	{
 		return true;
	}
	else
	{
		return false;
	}
}
