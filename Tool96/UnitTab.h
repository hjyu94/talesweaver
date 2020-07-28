#pragma once
#include "afxwin.h"

#include "Include.h"
#include "HJ_Map.h"
#include "MapTab.h"
#include "afxcmn.h"

// CUnitTab 대화 상자입니다.

class CUnitTab : public CDialog
{
	DECLARE_DYNAMIC(CUnitTab)

public:
	CUnitTab(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CMapTab*							m_pMapTab;
	CHJ_Map*							m_pMap;

	map<wstring, list<wstring>>			m_MapStrMap;
	map<wstring, list<wstring>>			m_UnitStrMap;

	vector<UNIT_INFO*>					m_vecUnit;

private: // contorl
	CComboBox		m_MapCombo;
	CListBox		m_MapList;
	
	CSliderCtrl		m_MapSlider;
	CStatic			m_ScaleTextCtrl;
	
	CComboBox		m_UnitCombo;
	CListBox		m_UnitList;
	
	CButton			m_AddBtn;
	CButton			m_DelBtn;
	CButton			m_SaveBtn;
	
	CListCtrl		m_CurUnitList;

public:
	enum LIST_COLUMN
	{
		LC_MAP, LC_OBJECT, LC_INDEX, LC_END
	};

public:
	void			Map_Initialize(void);
	void			Release(void);
	void			Add_Unit(CPoint& point);
	void			Delete_Unit(void);
	void			Load(void);
	void			Save(void);

public: // Set_Func
	void			Set_MapTab(CMapTab* pMapTab) { m_pMapTab = pMapTab; }
	void			Set_Map(CHJ_Map* pMap) { m_pMap = pMap; }

public:
	virtual BOOL OnInitDialog();
	afx_msg void On_Select_MapCombo();
	afx_msg void On_Select_Map();
	afx_msg void On_Select_UnitCombo();
	afx_msg void On_Select_Unit();
	afx_msg void On_Move_MapSlider(NMHDR *pNMHDR, LRESULT *pResult);
};
