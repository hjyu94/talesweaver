#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Sprite.h"
#include "TextureKeyDlg.h"

// CSpriteTab dialog

class CSpriteTab : public CDialogEx
{
	DECLARE_DYNAMIC(CSpriteTab)

public:
	CSpriteTab(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSpriteTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRITE_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private: // control
	CTreeCtrl				m_TreeCtrl;
	CListBox				m_PathListCtrl;
	CStatic					m_PathStrCtrl;
	CListBox				m_SpriteListCtrl;
	CStatic					m_PicCtrl;

	CEdit					m_ObjKeyCtrl;
	CEdit					m_StateKeyCtrl;

	CButton					m_PlayBtn;
	CButton					m_PauseBtn;

	CButton					m_ColorCheck;
	CEdit						m_RColorCtrl;
	CEdit						m_GColorCtrl;
	CEdit						m_BColorCtrl;
	CStatic					m_ColKeyPic;

	CButton					m_ReverseCheck;

	CSliderCtrl				m_SliderCtrl;
	CStatic					m_SpriteSpeedText;

	CButton					m_PosCheck;
	CEdit						m_PosXCtrl;
	CEdit						m_PosYCtrl;

	CButton					m_EditBtn;
	CButton					m_ResetBtn;
	CButton					m_DelBtn;
	CButton					m_LoadBtn;

	CButton					m_KeyCheckBox;
	CTextureKeyDlg		m_TextureKeyDlg;

private: // value
	CString					m_cstrObjKey = L"";
	CString					m_cstrStateKey = L"";
	CString					m_cstrDirPath = L""; 
	//// ..\\Texture\\Basket\\Basket%d
	// ..\\Texture\\Basket

	CSprite*				m_pSprite = nullptr;
	int						m_iSpriteCnt = 0;
	int						m_iSpriteSpeed = 100;

	BYTE					m_RValue = 0;
	BYTE					m_GValue = 0;
	BYTE					m_BValue = 0;

	bool					m_bIsTimerOn = false;;	// 프레임 스크롤 조절 시, 재생 여부

	//list<IMGPATH*>			m_PathVector;			// Texture
	vector<IMGPATH*>			m_PathVector;			// Texture
	map<CString, CImage*>	m_MapPngImage;		// HBitmap

	
public: // my func
	void						Make_Tree(const wstring& wstrPath, HTREEITEM& hParent);
	void						Update_Sprite_List_And_Cnt(const wstring& wstrPath);
	void						Load_Prev_Img(
									const CString& cstrPath, const CString& cstrObjKey, const CString& cstrStateKey
								);

	bool						Is_Reverse_Btn_Checked(void);
	bool						Is_Middle_Pos_Btn_Checked(void);
	bool						Is_ColorKey_Btn_Checked(void);
	void						Horizontal_Scroll(void);

	void						Load_Path_Data(void);
	
	void						Make_Path_List(HTREEITEM hSelected);
	IMGPATH*				Create_ImgPath(HTREEITEM hItem);
	void						Add_To_PathList(void);

public: // my func - get
	//list<IMGPATH*>*			Get_PathVectorPtr(void) { return &m_PathVector; }
	vector<IMGPATH*>*			Get_PathVectorPtr(void) { return &m_PathVector; }

public: // my func - set
	void					Set_Sprite(CSprite* pSprite) { m_pSprite = pSprite; }
	void					Set_Middle_Pos(POINT& point);

public:
	virtual BOOL			OnInitDialog();

	afx_msg void			On_Select_Tree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void			On_Select_Sprite();
	afx_msg void			On_Select_Path();

	afx_msg void			On_Click_Picture();

	afx_msg void			On_Play();
	afx_msg void			On_Pause();
	afx_msg void			OnTimer(UINT_PTR nIDEvent);

	afx_msg void			On_Reverse_Check();
	afx_msg void			On_Middle_Point_Check();

	afx_msg void			On_Move_Sprite_Speed_Slider(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void			On_Add();
	afx_msg void			On_Edit();

	afx_msg void			On_Copy();
	afx_msg void			On_Reset();
	afx_msg void			On_Delete();
	afx_msg void			On_Save();
};
