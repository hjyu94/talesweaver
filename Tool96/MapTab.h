#pragma once
#include "afxwin.h"

#include "FileInfo.h"
#include "afxcmn.h"
#include "TileInfo.h"

class CObj;
class CHJ_Map;

// CMapTab dialog

class CMapTab : public CDialog
{
	DECLARE_DYNAMIC(CMapTab)

public:
	CMapTab(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapTab();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAP_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	/**************************************************************************/

private: // control
		 //# map
	CListBox					m_MapCategoryList;
	CListBox					m_MapList;

	//# tile
	CListBox					m_TileList;
	CStatic						m_TilePicture;

	CSliderCtrl					m_MapScaleSlider;
	CStatic						m_MapScaleText;

	//# object
	CComboBox					m_ObjCombo;
	CListBox					m_ObjList;

	CStatic						m_AlphaText;
	CSliderCtrl					m_AlphaSlider;

	CStatic						m_ObjScaleText;
	CSliderCtrl					m_ObjScaleSlider;

	CStatic						m_ObjPicture;


private: // value
	map<wstring, list<wstring>>			m_MapMap;

	enum RADIO { R_OBJ, R_TILE, R_END };
	int									m_iRadio = 0;

	CHJ_Map*							m_pMap;
	map<CString, CImage*>				m_MapPngImage;
	int									m_iStageCnt;

	float								m_fMapScale;
	BYTE								m_bySelectedTile;

	//# Obj
	OBJ_DATA							m_tObjData;

	//# Dlg
	CTileInfo							m_TileInfo;

public: // my func
	void							HorizontalScroll(void);
	void							Initialize(void); // MainView 에서 호출한다
	void							On_LButton_Down_On_View(CPoint& point); // 스크롤 먹인 point를 인자로 넘김
	void							On_Mouse_Move(CPoint& point);
	void							Insert_Obj_Texture(wstring wstrStateKey, list<CString>* ItemListPtr);

public: // my func - Get
	float							Get_Scale(void) { return m_fMapScale; }
	BYTE							Get_Tile_Type(void) { return m_bySelectedTile; }
	map<wstring, list<wstring>>*	Get_MapStrMapPtr(void) { return &m_MapMap; }
	map<CString, CImage*>*			Get_ImageMapPtr(void) { return &m_MapPngImage; }
	float							Get_MapScale(void) { return m_fMapScale; }

public: // my func - Set
	void							Set_pHJ_Map(CHJ_Map* pHJ_Map) { m_pMap = pHJ_Map; }
	void							Set_Map_Size_Text(UINT width, UINT height);

public: // overriding
	virtual BOOL OnInitDialog();
	afx_msg void On_Reset_Obj();
	afx_msg void On_Reset_Tile();
	afx_msg void On_Save();

	//# 맵
	afx_msg void On_Select_Stage_Category();
	afx_msg void On_Select_Stage();
	afx_msg void On_Move_Map_Scale_Slider(NMHDR *pNMHDR, LRESULT *pResult);

	//# 오브젝트
	afx_msg void On_Move_Alpha_Slider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On_Move_Obj_Scale_Slider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On_Select_Object();
	afx_msg void On_Select_Object_Category();

	//# 타일
	afx_msg void On_Select_Tile_Type();
};
