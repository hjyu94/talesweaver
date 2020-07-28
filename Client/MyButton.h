#pragma once
#include "Obj.h"

class CMyButton
	: public CObj
{
public:
	CMyButton();
	virtual ~CMyButton();

public:
	virtual		HRESULT	Initialize(void);
	virtual		int		Progress(void);
	virtual		void	Render(void);
	virtual		void	Release(void);
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual void	Late_Progress(void) override;

public:
	void		Set_ObjKey(wstring wstrObjKey);
	void		Set_Pos(D3DXVECTOR3 vPos);
	void		Collision_Mouse(bool bCollision)
				{
					if (bCollision)
					{
						m_tFrame.iSpriteCnt = 1;
					}
					else
					{
						m_tFrame.iSpriteCnt = 0;
					}
				}

	bool bAlreadySound = false;
};

