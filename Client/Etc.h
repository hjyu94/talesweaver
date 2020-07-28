#pragma once
#include "Obj.h"
class CEtc :
	public CObj
{
public:
	CEtc();
	CEtc(wstring wstrStateKey, D3DXVECTOR3 vPos, int iCount);
	virtual ~CEtc();

private:
	wstring		m_wstrStateKey;
	D3DXVECTOR3	m_vPos;

public:
	// CObj을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Late_Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	int		m_iAlpha;
};

