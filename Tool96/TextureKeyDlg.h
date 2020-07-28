#pragma once

#include "Include.h"
#include "afxwin.h"
class CSpriteTab;

// CTextureKeyDlg 대화 상자입니다.

class CTextureKeyDlg : public CDialog
{
	DECLARE_DYNAMIC(CTextureKeyDlg)

public:
	CTextureKeyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTextureKeyDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXTUREKEYDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CComboBox		m_PathCombo;

	CEdit				m_BObjKeyEdit;
	CEdit				m_BStateKeyEdit;
	
	CEdit				m_ObjKeyEdit;
	CEdit				m_StateKeyEdit;

private:
	vector<IMGPATH*>*		m_vecPathPtr;
	CSpriteTab*					m_pSpriteTab;
	int								m_iCurComboIdx;
	
public:
	void							Set_SpriteTab(CSpriteTab* pTab) { m_pSpriteTab = pTab; }
	void							Init(void);
	
public:
	virtual BOOL				OnInitDialog();
	afx_msg void				On_Click_Ok();
	afx_msg void				On_Select_Path();
	afx_msg void				On_Killfocus_ObjEdit();
	afx_msg void				OnDestroy();
	afx_msg void On_Killfocus_StateEdit();
};
