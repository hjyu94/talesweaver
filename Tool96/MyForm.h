#pragma once

#include "afxcmn.h"

#include "MapTab.h"
#include "SpriteTab.h"
#include "UnitTab.h"
#include "PathFind.h"
#include "Tool96View.h"

// CMyForm 폼 뷰입니다.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	CTool96View*	m_pMainView;
	
	CTabCtrl		m_CtrlTab;
	CMapTab*		m_pMapDialog;
	CSpriteTab*		m_pSpriteTab;
	CUnitTab*		m_pUnitTab;
	CPathFind*		m_pPathFind;

public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};


