#pragma once
#include "UI.h"
class CChat :
	public CUI
{
public:
	CChat();
	virtual ~CChat();

public:
	// CUI��(��) ���� ��ӵ�
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	enum SCROLL {SCR_UP, SCR_BACK, SCR_DOWN, SCR_END};
	
public:
	list<wstring>*		Get_ChatListPtr(void) { return &m_ChatList; }
	bool				Was_Entered_Text(void) { return m_bTextPush; }
	bool				Is_Editing() { return m_bIsEditing; }

private:
	// �Է�
	bool				m_bIsEditing;
	bool				m_bCapsLock; // true �빮��, false �ҹ���
	
	wstring				m_wstrText;
	D3DXVECTOR3			m_InputPos;
	RECT				m_InputRect;
	
	// scroll
	D3DXVECTOR3			m_ScrollPos[SCR_END];
	RECT				m_ScrollRect[SCR_END];

	// ä�� â
	int					m_iStartIndex = 0;
	D3DXVECTOR3			m_ChatPos[4];
	RECT				m_ChatRect[4];
	list<wstring>		m_ChatList;

	// Ű �Է�
	char				m_arrUpperCase[26];
	char				m_arrLowerCase[26];

	// �Է��� ��� ��õ��� �÷��̾� ���� ����
	float				m_fTime;
	D3DXVECTOR3			m_vTextCenter;
	RECT				m_rcText;

	// �ٸ� NPC�� �÷��̾ �Է��ߴ��� Ȯ���ϱ�
	bool				m_bTextPush;
};

