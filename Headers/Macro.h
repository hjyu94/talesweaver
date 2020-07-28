#pragma once

#define NO_COPY(ClassName)												\
private:																\
	ClassName(const ClassName& obj);									\
	ClassName& operator=(const ClassName& obj);

#define DECLARE_SINGLETON(ClassName)									\
		NO_COPY(ClassName)												\
public:																	\
	static ClassName* Get_Instance()									\
	{																	\
		if(nullptr == m_pInstance)										\
			m_pInstance = new ClassName;								\
																		\
		return m_pInstance;												\
	}																	\
																		\
	void Destroy_Instance()												\
	{																	\
		if(m_pInstance)													\
		{																\
			delete m_pInstance;											\
			m_pInstance = nullptr;										\
		}																\
	}																	\
																		\
private:																\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)									\
ClassName* ClassName::m_pInstance = nullptr;

#define ERR_MSG(Message)		MessageBox(g_hWnd, Message, L"System Error", NULL)

#define GET_TIME				CTimeMgr::Get_Instance()->GetTime()

