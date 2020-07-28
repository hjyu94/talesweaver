
// Tool96View.h : CTool96View Ŭ������ �������̽�
//

#pragma once

class CGraphicDev;
class CTextureMgr;
class CTool96Doc;

 
#include "Obj.h"
#include "HJ_Map.h"
#include "Sprite.h"

class CTool96View : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CTool96View();
	DECLARE_DYNCREATE(CTool96View)

// Ư���Դϴ�.
public:
	CTool96Doc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CTool96View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CGraphicDev*		m_pGraphicDev = nullptr;
	CTextureMgr*		m_pTextureMgr = nullptr;

public:
	CHJ_Map*			m_pMap = nullptr;
	CSprite*				m_pSprite = nullptr;

private:
	TAB					m_eTab;
	bool					m_bLButtonClick;

public:
	void					Set_eTab(TAB _tab) { m_eTab = _tab; }
	TAB					Get_eTab() { return m_eTab; }

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnMouseHover(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // Tool96View.cpp�� ����� ����
inline CTool96Doc* CTool96View::GetDocument() const
   { return reinterpret_cast<CTool96Doc*>(m_pDocument); }
#endif

