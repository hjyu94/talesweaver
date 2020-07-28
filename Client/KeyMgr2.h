#pragma once
#define VK_MAX 0xff

class CKeyMgr2
{
public:
private:
	CKeyMgr2();
	~CKeyMgr2();
public:
	static CKeyMgr2* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CKeyMgr2; 
		return m_pInstance; 
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance; 
			m_pInstance = nullptr; 
		}
	}
public:
	bool KeyPressing(int iKey);
	bool KeyDown(int iKey);
	bool KeyUP(int iKey);
	void KeyUpdate();

private:
	static CKeyMgr2* m_pInstance;
	bool m_bKeyState[VK_MAX];
};

