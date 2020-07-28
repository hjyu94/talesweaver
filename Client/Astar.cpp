#include "stdafx.h"
#include "Astar.h"
#include "ObjMgr.h"
//#include "BackGround.h"
#include "Back.h"

IMPLEMENT_SINGLETON(CAstar)

// Min Cost 를 찾기 위한 조건자
bool Compare(NODE* pNode1, NODE* pNode2)
{
	if (pNode1->fCost < pNode2->fCost) 
	{
		return true;
	}
	else
	{
		return false;
	}
}


CAstar::CAstar()
{
}


CAstar::~CAstar()
{
	Release();
}

void CAstar::Astar_Start(int iStart_Idx, int iDst_Idx)
{
	const vector<TILE*>* vecTile = 
		static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background())
		->Get_TileVecPtr();
	Release();

	//1) 도착지가 벡터 범위 밖이거나
	if (iDst_Idx >= vecTile->size())
		return;
	
	//2) 출발지와 도착지가 같거나
	if (iStart_Idx == iDst_Idx)
		return;

	//3) 도착지가 갈 수 없는 지역이거나
	if (TO_NO_MOVE == (*vecTile)[iDst_Idx]->eOption)
		return;

	m_iStart_Idx = iStart_Idx;
	m_iDst_Idx = iDst_Idx;

	Make_Route();
}

void CAstar::Make_Route(void)
{
	//## 시작 인덱스를 갖는 노드 만들기
	NODE*	pParent = new NODE;
	
	pParent->iIndex = m_iStart_Idx;
	pParent->fCost = 0.f;
	pParent->pPrev = nullptr;

	m_CloseList.push_back(pParent);

	//## 갈 수 있는 8개의 타일 만들기
	NODE*	pNode = nullptr;

	// 확인, 질문할 것
	CBack* back = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background());
	const vector<TILE*>* pTileVec = back->Get_TileVecPtr();

	int iIndex = -1;

	while (true)
	{
		D3DXVECTOR3 vPos = (*pTileVec)[pParent->iIndex]->vPos;

		//1. 위쪽 타일
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x, vPos.y - TILECY, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}

		//2. 아래쪽 타일
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x, vPos.y + TILECY, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}


		//3. 왼쪽 타일
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x - TILECX, vPos.y, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}

		//4. 오른쪽 타일
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x + TILECX, vPos.y, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}

		//5. 왼쪽 위 타일
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x - TILECX*0.5f, vPos.y - TILECY*0.5f, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}

		//6  오른쪽 위 타일
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x + TILECX*0.5f, vPos.y - TILECY*0.5f, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}

		//7. 왼쪽 아래 타일
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x - TILECX*0.5f, vPos.y + TILECY*0.5f, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}


		//8. 오른쪽 아래 타일
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x + TILECX*0.5f, vPos.y + TILECY*0.5f, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}

		m_OpenList.sort(Compare);

		auto	iter = m_OpenList.begin();
		pParent = *iter;

		m_CloseList.push_back(*iter);
		m_OpenList.erase(iter);

		if (m_iDst_Idx == pParent->iIndex)
		{
			while (true)
			{
				m_BestList.push_front(pParent->iIndex);
				pParent = pParent->pPrev;

				if (m_iStart_Idx == pParent->iIndex) 
					break;
			}
			break;
		}
	}

	int i = 0;
}

void CAstar::Release()
{
	for_each(m_OpenList.begin(), m_OpenList.end(), DeleteObj());
	m_OpenList.clear();

	for_each(m_CloseList.begin(), m_CloseList.end(), DeleteObj());
	m_CloseList.clear();

	m_BestList.clear();
}

NODE* CAstar::Make_Node(int iIdx, NODE * pPrev, const vector<TILE*>* pTileVec)
{
	NODE* pNode = new NODE;
	pNode->iIndex = iIdx;
	pNode->pPrev = pPrev;
	
	// 출발지 ~ 타일
	D3DXVECTOR3 vDist = (*pTileVec)[iIdx]->vPos - (*pTileVec)[m_iStart_Idx]->vPos;
	float fPCost = D3DXVec3Length(&vDist);
	
	// 타일 ~ 목적지
	vDist = (*pTileVec)[m_iDst_Idx]->vPos - (*pTileVec)[iIdx]->vPos;
	float fGCost = D3DXVec3Length(&vDist);

	pNode->fCost = fPCost + fGCost;

	return pNode;
}

bool CAstar::Check_List(int iIdx)
{
	// OpenList, CloseList 에 이미 있다면 false 반환

	for (auto& pNode : m_OpenList)
	{
		if (pNode->iIndex == iIdx) return false;
	}

	for (auto& pNode : m_CloseList)
	{
		if (pNode->iIndex == iIdx) return false;
	}

	return true;
}
