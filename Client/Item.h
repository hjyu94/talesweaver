#pragma once
#include "Obj.h"
class CItem :
	public CObj
{
public:
	CItem();
	CItem(ITEM_INFO tInfo);
	virtual ~CItem();

public:
	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual void Late_Progress(void) override;


public:
	void		Set_TextureKey(wstring wstrObjKey, wstring wstrStateKey = L"");
	ITEM_INFO&	Get_ItemInfo() { return m_tItem; }
	
protected:
	ITEM_INFO	m_tItem;
};

