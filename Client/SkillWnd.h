#pragma once
#include "WndUI.h"
class CSkillIcon;

class CSkillWnd :
	public CWndUI
{
public:
	CSkillWnd();
	virtual ~CSkillWnd();

	// CWndUI을(를) 통해 상속됨
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

public:
	D3DXVECTOR3		m_vSkillPos[3][10];
	RECT			m_vSkillRect[3][10];
	CSkillIcon*		m_wstrSkill[3][10];
};

