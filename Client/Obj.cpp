#include "stdafx.h"
#include "Obj.h"
#include "ObjMgr.h"
#include "DataSubject.h"

D3DXVECTOR3 CObj::m_vScroll = {};

CObj::CObj()
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
}

CObj::~CObj()
{
}

void CObj::Set_Pos(D3DXVECTOR3 vPos)
{
	m_tInfo.vPos = vPos;
}

void CObj::Set_Pos(int iTileIdx)
{
	vector<TILE*>* pTileVec = static_cast<CBack*>(CObjMgr::Get_Instance()->Get_Background())->Get_TileVecPtr();
	m_tInfo.vPos = (*pTileVec)[iTileIdx]->vPos;
}
