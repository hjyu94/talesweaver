#pragma once

//class CObj;
#include "Obj.h"

template <typename T>
class CObjFactory
{
public:
	static T* CreateObj()
	{
		T* pObj = new T;
		pObj->Initialize();
		return pObj;
	}

	static T* CreateObj(D3DXVECTOR3 vPos)
	{
		T* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(vPos);
		return pObj;
	}

	static T* CreateObj(int iTileIdx)
	{
		T* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(iTileIdx);
		return pObj;
	}
};
