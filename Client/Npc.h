#pragma once
#include "Obj.h"
#include "ShopWnd.h"
class CNpc :
	public CObj
{
public:
	CNpc();
	CNpc(wstring wstrName);
	virtual ~CNpc();

public:
	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Late_Progress(void) override;

public:
	vector<CItem*>*	Get_InvenPtr() { return &m_vecShop; }
	void			Add_Item(CItem* pItem) { m_vecShop.push_back(pItem); }

private:
	CShopWnd*		m_pShopWnd;
	vector<CItem*>	m_vecShop;

};

