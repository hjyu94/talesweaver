#pragma once

#include "Include.h"

class CAstar
{
	DECLARE_SINGLETON(CAstar)

private:
	CAstar();
	~CAstar();

public:
	void		Astar_Start(int iStart_Idx, int iDst_Idx);
	void		Make_Route(void);
	void		Release();

	NODE*		Make_Node(int iIdx, NODE* pPrev, const vector<TILE*>* pTileVec);
	bool		Check_List(int iIdx);

public:
	list<int> Get_BestList() { return m_BestList; }

private:
	list<NODE*> m_OpenList;
	list<NODE*> m_CloseList;
	list<int>	m_BestList;
	
	int			m_iStart_Idx;
	int			m_iDst_Idx;

};

