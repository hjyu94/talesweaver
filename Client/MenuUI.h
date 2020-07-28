#pragma once
#include "UI.h"
#include "WndUI.h"

class CMenuUI :
	public CUI
{
public:
	CMenuUI();
	virtual ~CMenuUI();

public:
	// CUI을(를) 통해 상속됨
	virtual HRESULT Initialize(void) override;
	virtual int Progress(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;
	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);


public:
	enum PART
	{
		PT_STATUS, PT_EQUIP, PT_INVEN, PT_SKILL, PT_EVENT, PT_END
	};
	
public:
	void			Reset_Priority(CWndUI* pWnd);

public:
	CWndUI*			Get_ActiveUI(void);
	CWndUI*			Get_UI(PART ePart) { return m_Wnd[ePart]; }
	void			Show_ShopWnd(void);

private:
	static int		m_iPriority;

	bool			m_bExtended;

	D3DXVECTOR3		m_vDownPos;
	D3DXMATRIX		m_matDownWorld;
	RECT			m_rcDown;

	D3DXVECTOR3		m_vUpPos;
	D3DXMATRIX		m_matUpWorld;
	RECT			m_rcUp;

	bool			m_bPartClicked[PT_END];
	wstring			m_wstrStateKey[PT_END];
	D3DXVECTOR3		m_vPartPos[PT_END];
	RECT			m_rcPart[PT_END];
	D3DXMATRIX		m_matWorld[PT_END];

	CWndUI*			m_Wnd[PT_END];		 // ObjMgr에 있는 포인터를 들고 있다.
	list<CWndUI*>	m_RenderSort;		// 원소를 지워버리면 안되고 clear()만 해야 한다.
};

//Button | BuyButton | 2 | ..\Texture\UI\Button\BuyButton\BuyButton%d.png | 100 | FALSE | X | X
//Button | CancelButton | 2 | ..\Texture\UI\Button\CancelButton\CancelButton%d.png | 100 | FALSE | X | X
//Button | CloseButton | 2 | ..\Texture\UI\Button\CloseButton\CloseButton%d.png | 100 | FALSE | X | X
//Button | EquipButton | 2 | ..\Texture\UI\Button\EquipButton\EquipButton%d.png | 100 | FALSE | X | X
//Button | ExitButton | 2 | ..\Texture\UI\Button\ExitButton\ExitButton%d.png | 100 | FALSE | X | X
//Button | InvenButton | 2 | ..\Texture\UI\Button\InvenButton\InvenButton%d.png | 100 | FALSE | X | X
//Button | OKButton | 2 | ..\Texture\UI\Button\OKButton\OKButton%d.png | 100 | FALSE | X | X
//Button | SellButton | 2 | ..\Texture\UI\Button\SellButton\SellButton%d.png | 100 | FALSE | X | X
//Button | SkillButton | 2 | ..\Texture\UI\Button\SkillButton\SkillButton%d.png | 100 | FALSE | X | X
//Button | StatusButton | 2 | ..\Texture\UI\Button\StatusButton\StatusButton%d.png | 100 | FALSE | X | X
//Button | DownButton | 2 | ..\Texture\UI\Button\DownButton\Down%d.png | 100 | FALSE | X | X
//Button | UpButton | 2 | ..\Texture\UI\Button\UpButton\Up%d.png | 100 | FALSE | X | X
//Button | MaxButton | 2 | ..\Texture\UI\Button\MaxButton\MaxBtn_%d.png | 100 | FALSE | X | X
