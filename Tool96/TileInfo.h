#pragma once
#include "afxwin.h"

class CMapTab;
class CHJ_Map;
// CTileInfo dialog

class CTileInfo : public CDialog
{
	DECLARE_DYNAMIC(CTileInfo)

public:
	CTileInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTileInfo();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TILEINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CComboBox		m_CategoryCombo;
	CListBox		m_MapListBox;
	CStatic			m_MapPicCtrl;

private:
	CMapTab*		m_pMapTab;
	CHJ_Map*		m_pMap;
	D3DXVECTOR3		m_vPoint;

public:
	afx_msg void	On_Select_Category();
	virtual BOOL	OnInitDialog();
	afx_msg void	On_Select_Map();
	afx_msg void	On_Click_Ok();

public:
	void			Set_Map_Tab(CMapTab* pMapTab) { m_pMapTab = pMapTab; }
	void			Set_Point(D3DXVECTOR3 vPoint) { m_vPoint = vPoint; }
	void			Set_Map(CHJ_Map* pMap) { m_pMap = pMap; }
};
