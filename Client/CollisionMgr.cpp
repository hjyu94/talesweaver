#include "stdafx.h"
#include "CollisionMgr.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CollisionRect(OBJLIST & DestList, OBJLIST & SourList)
{
	RECT rc = {}; 
	for (auto& pDst : DestList)
	{
		for (auto& pSrc : SourList)
		{
			if (IntersectRect(&rc, &pDst->Get_Rect(), &pSrc->Get_Rect()))
			{
				// ...
			}
		}
	}
}

// CCollisionMgr::CollisionRect(OBJ_PLAYER, OBJ_MONSTER);
void CCollisionMgr::CollisionRect(OBJID DstID, OBJID SrcID)
{
	OBJLIST DstList = *CObjMgr::Get_Instance()->Get_ObjListPtr(DstID);
	OBJLIST SrcList = *CObjMgr::Get_Instance()->Get_ObjListPtr(SrcID);

	RECT rc = {};
	bool bIsColl = false;
	
	for (CObj* pDst : DstList)
	{
		for (CObj* pSrc : SrcList)
		{
			if ((DstID == OBJ_PLAYER) && (SrcID == OBJ_MONSTER))
			{
				CBattleUnit* pUnit = static_cast<CBattleUnit*>(pDst);
				if (pUnit->Is_Shield())
				{
					Push_Src(pDst, pSrc);
				}
			}
		}
	}
}

void CCollisionMgr::Push_Src(CObj * pDst, CObj * pSrc)
{
	float fMoveX = 0.f, fMoveY = 0.f;

	if (CheckRect(pDst, pSrc, &fMoveX, &fMoveY))
	{
    	float x = pSrc->Get_Info().vPos.x;
		float y = pSrc->Get_Info().vPos.y;

		if (fMoveX > fMoveY)
		{
			if (y < pDst->Get_Info().vPos.y)
				fMoveY *= -1.f;

			pSrc->Set_Pos(vec{ x, y + fMoveY, 0.f });
		}
		else
		{
			if (x < pDst->Get_Info().vPos.x)
				fMoveX *= -1.f;

			pSrc->Set_Pos(vec{ x + fMoveX, y, 0.f });
		}
	}
}

//void CCollisionMgr::CollisionRectEX(OBJLIST & DestList, OBJLIST & SourList)
//{
//	float fMoveX = 0.f, fMoveY = 0.f; 
//	for (auto& pDst : DestList)
//	{
//		for (auto& pSrc : SourList)
//		{
//			if (CheckRect(pDst, pSrc, &fMoveX, &fMoveY))
//			{
//				float x = pSrc->Get_Info().fX;
//				float y = pSrc->Get_Info().fY; 
//
//				if (fMoveX > fMoveY)
//				{
//					if (y < pDst->Get_Info().fY)
//						fMoveY *= -1.f;
//
//					pSrc->Set_Pos(x, y + fMoveY);
//				}
//				else
//				{
//					if (x < pDst->Get_Info().fX)
//						fMoveX *= -1.f; 
//
//					pSrc->Set_Pos(x + fMoveX, y);
//				}
//			}
//		}
//	}
//
//}
//
//void CCollisionMgr::CollisionSphere(OBJLIST & DestList, OBJLIST & SourList)
//{
//	for (auto& pDst : DestList)
//	{
//		for (auto& pSrc : SourList)
//		{
//			if (CheckSphere(pDst, pSrc))
//			{
//				pDst->Set_Dead();
//				pSrc->Set_Dead();
//			}
//		}
//	}
//}
//
//bool CCollisionMgr::CheckSphere(CObj * pDst, CObj * pSrc)
//{
//
//	// 반지름의 합 구하기. 
//	float fRadiusSum = (pDst->Get_Info().fCX * 0.5f) + (pSrc->Get_Info().fCX * 0.5f);
//	// 밑변과 높이 구하기. 
//	float fx = pDst->Get_Info().fX - pSrc->Get_Info().fX;
//	float fy = pDst->Get_Info().fY - pSrc->Get_Info().fY;
//
//	// 피타고라스의 정리를 사용하여 거리를 구하기. 
//	float fDist = sqrtf(fx * fx + fy * fy); // a^2 + b^2 = fDist;
//	//거리와 반지름의 합으로 충돌판정. 
//	return fDist <= fRadiusSum;
//}
//
bool CCollisionMgr::CheckRect(CObj * pDst, CObj * pSrc, float* pMoveX, float* pMoveY)
{
	// 반지름의 합. 
	float pSrcCX = (pSrc->Get_Rect().right - pSrc->Get_Rect().left);
	float pSrcCY = (pSrc->Get_Rect().bottom - pSrc->Get_Rect().top);

	float pDstCX = (pDst->Get_Rect().right - pDst->Get_Rect().left);
	float pDstCY = (pDst->Get_Rect().bottom - pDst->Get_Rect().top);


	float fRadiusSumX = (pDstCX * 0.5f) + (pSrcCX * 0.5f);
	float fRadiusSumY = (pDstCY * 0.5f) + (pSrcCY * 0.5f);

	float fDistX = fabs(pDst->Get_Info().vPos.x - pSrc->Get_Info().vPos.x);
	float fDistY = fabs(pDst->Get_Info().vPos.y - pSrc->Get_Info().vPos.y); 

	if (fRadiusSumX >= fDistX && fRadiusSumY >= fDistY)
	{
		*pMoveX = fRadiusSumX - fDistX;
		*pMoveY = fRadiusSumY - fDistY;
		return true; 
	}

	return false;
}
