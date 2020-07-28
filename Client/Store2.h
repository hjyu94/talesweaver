#pragma once
#include "Scene.h"

class CNpc;
class CEventWnd;
class CMenuUI;

class CStore2 :
	public CScene
{
public:
	CStore2();
	virtual ~CStore2();

public:
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Late_Progress(void) override;

private:
	list<wstring>*		m_ChatListPointer = nullptr;
	int					m_iRevivePoint = 0;

	// NPC
	CMenuUI*			m_pMenuUI = nullptr;
	CEventWnd*			m_pEventWnd = nullptr;
	RECT				m_rcNPC;
	bool				m_bQuestShow = false;

	// NPC ¸»Ç³¼±
	float				m_fTime;
	CNpc*				m_pNpc;
	bool				m_bTextShow = false;
	bool				m_bEnteredText;
	D3DXVECTOR3			m_vTextCenter;
	RECT				m_rcText;

	bool				m_bQuestClear = false;
};

