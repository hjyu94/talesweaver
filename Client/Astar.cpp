#include "stdafx.h"
#include "Astar.h"
#include "ObjMgr.h"
//#include "BackGround.h"
#include "Back.h"

IMPLEMENT_SINGLETON(CAstar)

// Min Cost �� ã�� ���� ������
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

	//1) �������� ���� ���� ���̰ų�
	if (iDst_Idx >= vecTile->size())
		return;
	
	//2) ������� �������� ���ų�
	if (iStart_Idx == iDst_Idx)
		return;

	//3) �������� �� �� ���� �����̰ų�
	if (TO_NO_MOVE == (*vecTile)[iDst_Idx]->eOption)
		return;

	m_iStart_Idx = iStart_Idx;
	m_iDst_Idx = iDst_Idx;

	Make_Route();
}

void CAstar::Make_Route(void)
{
	//## ���� �ε����� ���� ��� �����
	NODE*	pParent = new NODE;
	
	pParent->iIndex = m_iStart_Idx;
	pParent->fCost = 0.f;
	pParent->pPrev = nullptr;

	m_CloseList.push_back(pParent);

	//## �� �� �ִ� 8���� Ÿ�� �����
	NODE*	pNode = nullptr;

	// Ȯ��, ������ ��
	CBack* back = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background());
	const vector<TILE*>* pTileVec = back->Get_TileVecPtr();

	int iIndex = -1;

	while (true)
	{
		D3DXVECTOR3 vPos = (*pTileVec)[pParent->iIndex]->vPos;

		//1. ���� Ÿ��
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x, vPos.y - TILECY, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}

		//2. �Ʒ��� Ÿ��
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x, vPos.y + TILECY, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}


		//3. ���� Ÿ��
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x - TILECX, vPos.y, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}

		//4. ������ Ÿ��
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x + TILECX, vPos.y, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}

		//5. ���� �� Ÿ��
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x - TILECX*0.5f, vPos.y - TILECY*0.5f, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}

		//6  ������ �� Ÿ��
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x + TILECX*0.5f, vPos.y - TILECY*0.5f, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}

		//7. ���� �Ʒ� Ÿ��
		iIndex = back->Get_Tile_Index(D3DXVECTOR3(vPos.x - TILECX*0.5f, vPos.y + TILECY*0.5f, 0.f));
		if (iIndex != -1 && (*pTileVec)[iIndex]->eOption != 1 && Check_List(iIndex))
		{
			NODE* pNode = Make_Node(iIndex, pParent, pTileVec);
			m_OpenList.push_back(pNode);
		}


		//8. ������ �Ʒ� Ÿ��
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
	
	// ����� ~ Ÿ��
	D3DXVECTOR3 vDist = (*pTileVec)[iIdx]->vPos - (*pTileVec)[m_iStart_Idx]->vPos;
	float fPCost = D3DXVec3Length(&vDist);
	
	// Ÿ�� ~ ������
	vDist = (*pTileVec)[m_iDst_Idx]->vPos - (*pTileVec)[iIdx]->vPos;
	float fGCost = D3DXVec3Length(&vDist);

	pNode->fCost = fPCost + fGCost;

	return pNode;
}

bool CAstar::Check_List(int iIdx)
{
	// OpenList, CloseList �� �̹� �ִٸ� false ��ȯ

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
