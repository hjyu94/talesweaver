#pragma once
#include "Obj.h"
class CSkill :
	public CObj
{
public:
	CSkill();
	virtual ~CSkill();

public:	
	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual void	Late_Progress(void) override;
};

