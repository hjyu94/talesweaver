#include "stdafx.h"
#include "ObjMgr.h"
#include "ObjFactory.h"

IMPLEMENT_SINGLETON(CObjMgr)

CObjMgr::CObjMgr()
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::AddObject(OBJID eID, CObj* pObj)
{
	m_ObjList[eID].push_back(pObj);
}

HRESULT CObjMgr::Initialize(void)
{
	return S_OK;
}

int CObjMgr::Progress(void)
{
	int iResult = 0;

	//for (size_t i = 0; i < OBJ_END; ++i)
	//{
	//	for (auto& iter : m_ObjList[i])
	//	{
	//		iResult = iter->Progress();

	//		if (m_ObjList[i].empty())
	//			break;
	//	}
	//}
	
	// 마우스 상태 리셋
	CMouse* pMouse = static_cast<CMouse*>(Get_Mouse());
	pMouse->Set_State(CMouse::ON::ON_ITEM, false);
	pMouse->Set_State(CMouse::ON::ON_MONSTER, false);
	pMouse->Set_State(CMouse::ON::ON_UI, false);
	
	CPlayer* pPlayer = static_cast<CPlayer*>(Get_Player());
	pPlayer->Set_State(CPlayer::ON_OBJECT, false);
	
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end(); )
		{
			iResult = (*iter)->Progress();

			if (1 == iResult)
			{
				delete *iter;
				iter = m_ObjList[i].erase(iter);
			}
			else
				++iter;
		}
	}

	return iResult;
}

void CObjMgr::Late_Progress(void)
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_ObjList[i])
		{
			pObj->Late_Progress();

			RENDERID eRenderID = pObj->Get_RenderID();
			m_RenderSort[eRenderID].emplace_back(pObj);
		}
	}
}

void CObjMgr::Render(void)
{
	/*for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Render();
	}*/

	for (int i = 0; i < RDR_END; ++i)
	{
		m_RenderSort[i].sort(CompareY<CObj*>);
		for (auto& pObj : m_RenderSort[i])
		{
			pObj->Render();
		}
		m_RenderSort[i].clear();
	}
}

void CObjMgr::Release(void)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end(); ++iter)
		{
			if (*iter != nullptr)
				delete *iter;
		}
		m_ObjList[i].clear();
	}
}

void CObjMgr::DeleteObject(OBJID eID)
{
	for_each(m_ObjList[eID].begin(), m_ObjList[eID].end(), DeleteObj());
	m_ObjList[eID].clear();
}

CObj * CObjMgr::Get_Background(void)
{
	if (!m_ObjList[OBJ_BACK].empty())
		return m_ObjList[OBJ_BACK].front();
	else
		return nullptr;
}

CObj * CObjMgr::Get_Player(void)
{
	if (!m_ObjList[OBJ_PLAYER].empty())
		return m_ObjList[OBJ_PLAYER].front();
	else
		return nullptr;
}

CObj * CObjMgr::Get_Mouse(void)
{
	if (!m_ObjList[OBJ_MOUSE].empty())
		return m_ObjList[OBJ_MOUSE].front();
	else
		return nullptr;
}

CMouse::STATE CObjMgr::Get_Mouse_State(void)
{
	return static_cast<CMouse*>(m_ObjList[OBJ_MOUSE].front())->Get_State();
}

list<CObj*>* CObjMgr::Get_ObjListPtr(OBJID eID)
{
	return &m_ObjList[eID];
}

//const vector<TILE*>* CObjMgr::Get_TileVec(void)
//{
//	return static_cast<CBackGround*>(m_ObjList[OBJ_BACK].front())->Get_TileVecPtr();
//}

//void CObjMgr::Get_Map_Size(int & iWidth, int & iHeight)
//{
//	static_cast<CBackGround*>(m_ObjList[OBJ_BACK].front()
//}
