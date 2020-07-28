#pragma once

#include "afxcmn.h"

#include "MapTab.h"
#include "SpriteTab.h"
#include "UnitTab.h"
#include "PathFind.h"
#include "Tool96View.h"

// CMyForm �� ���Դϴ�.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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


