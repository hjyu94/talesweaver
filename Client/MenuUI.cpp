#include "stdafx.h"
#include "MenuUI.h"
#include "ObjMgr.h"
#include "ObjFactory.h"

int CMenuUI::m_iPriority = 0;

CMenuUI::CMenuUI()
	: m_bExtended(true)
{
	ZeroMemory(&m_bPartClicked, sizeof(bool) * PT_END);
	ZeroMemory(&m_vPartPos, sizeof(D3DXVECTOR3) * PT_END);
	ZeroMemory(&m_rcPart, sizeof(RECT) * PT_END);
	ZeroMemory(&m_matWorld, sizeof(D3DXMATRIX) * PT_END);
	//todo
	//...

	CWndUI* pWnd = nullptr;
	
	pWnd = CObjFactory<CStatusWnd>::CreateObj();
	pWnd->Set_Menu(this);
	pWnd->Set_Priority(-1);
	m_Wnd[PT_STATUS] = pWnd;
	m_RenderSort.push_back(pWnd);

	pWnd = CObjFactory<CEquipWnd>::CreateObj();
	pWnd->Set_Menu(this);
	pWnd->Set_Priority(-1);
	m_Wnd[PT_EQUIP] = pWnd;
	m_RenderSort.push_back(pWnd);

	pWnd = CObjFactory<CInvenWnd>::CreateObj();
	pWnd->Set_Menu(this);
	pWnd->Set_Priority(-1);
	m_Wnd[PT_INVEN] = pWnd;
	m_RenderSort.push_back(pWnd);

	pWnd = CObjFactory<CSkillWnd>::CreateObj();
	pWnd->Set_Menu(this);
	pWnd->Set_Priority(-1);
	m_Wnd[PT_SKILL] = pWnd;
	m_RenderSort.push_back(pWnd);

	pWnd = CObjFactory<CEventWnd>::CreateObj();
	pWnd->Set_Menu(this);
	pWnd->Set_Priority(-1);
	m_Wnd[PT_EVENT] = pWnd;
	m_RenderSort.push_back(pWnd);

	//pWnd = CObjFactory<CShopWnd>::CreateObj();
	//pWnd->Set_Menu(this);
	//pWnd->Set_Priority(-1);
	//m_Wnd[PT_SHOP] = pWnd;
	//m_RenderSort.push_back(pWnd);
}


CMenuUI::~CMenuUI()
{
	Release();
}

HRESULT CMenuUI::Initialize(void)
{
	// UI 메인 버튼
	m_wstrStateKey[PT_STATUS] = L"StatusButton";
	m_wstrStateKey[PT_EQUIP] = L"EquipButton";
	m_wstrStateKey[PT_INVEN] = L"InvenButton";
	m_wstrStateKey[PT_SKILL] = L"SkillButton";
	m_wstrStateKey[PT_EVENT] = L"EventButton";
	
	m_vPartPos[PT_STATUS] = D3DXVECTOR3(WINCX - 18.f, 0.f, 0.f);
	m_vPartPos[PT_EQUIP] = D3DXVECTOR3(WINCX - 18.f, 51.f, 0.f);
	m_vPartPos[PT_INVEN] = D3DXVECTOR3(WINCX - 18.f, 102.f, 0.f);
	m_vPartPos[PT_SKILL] = D3DXVECTOR3(WINCX - 18.f, 153.f, 0.f);
	m_vPartPos[PT_EVENT] = D3DXVECTOR3(WINCX - 18.f, 204.f, 0.f);

	D3DXMATRIX matTrans;

	for (int i = 0; i < PT_END; ++i)
	{
		D3DXMatrixTranslation(&matTrans, m_vPartPos[i].x, m_vPartPos[i].y, 0.f);
		m_matWorld[i] = matTrans;
	}

	// 메뉴 접는 Up 버튼
	m_vUpPos = D3DXVECTOR3(WINCX - 18.f, 255.f, 0.f);
	D3DXMatrixTranslation(
		&matTrans, m_vUpPos.x, m_vUpPos.y, 0.f);
	m_matUpWorld = matTrans;

	// 메뉴 접혔을때 펴주는 down 버튼
	m_vDownPos = D3DXVECTOR3(WINCX - 18.f, 0.f, 0.f);
	D3DXMatrixTranslation(
		&matTrans, m_vDownPos.x, m_vDownPos.y, 0.f);
	m_matDownWorld = matTrans;

	return S_OK;
}

int CMenuUI::Progress(void)
{
	CMouse* pMouse = static_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
	//pMouse->Set_State(CMouse::ON_UI, false);

	D3DXVECTOR3 vMouse = ::GetMouse();
	POINT pt = { LONG(vMouse.x), LONG(vMouse.y) };

	if (m_bExtended)
	{
		for (int i = 0; i < PT_END; ++i)
		{
			if (PtInRect(&m_rcPart[i], pt))
			{
				pMouse->Set_State(CMouse::ON_UI, true);

				if (CKeyMgr::Get_Instance()->KeyDown(VK_LBUTTON, CKeyMgr::S_UI_MENU))
				{
					m_bPartClicked[i] = true;
					m_Wnd[i]->Show(true);
					m_Wnd[i]->Set_Priority(m_iPriority);
					m_iPriority++;
					CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::EFFECT);
					break;
				}
			}
		}

		if (PtInRect(&m_rcUp, pt))
		{
			pMouse->Set_State(CMouse::ON_UI, true);
			if (CKeyMgr::Get_Instance()->KeyDown(VK_LBUTTON, CKeyMgr::S_UI_MENU))
			{
				m_bExtended = false;
				CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::EFFECT);
			}
		}
	}
	else
	{
		if (PtInRect(&m_rcDown, pt))
		{
			pMouse->Set_State(CMouse::ON_UI, true);
			if (CKeyMgr::Get_Instance()->KeyDown(VK_LBUTTON, CKeyMgr::S_UI_MENU))
			{
				m_bExtended = true;
				CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::UI);
			}
		}
	}

	
	for (int i = 0; i < PT_END; ++i)
	{
		m_Wnd[i]->Progress();
	}

	return 0;
}

void CMenuUI::Render(void)
{
	// 메뉴 열려있을 때
	if (m_bExtended)
	{
		for (int i = 0; i < PT_END; ++i)
		{
			const TEXINFO* pTexture
				= CTextureMgr::Get_Instance()->GetTexture(L"Button", m_wstrStateKey[i], m_bPartClicked[i]);

			if (nullptr == pTexture)
				break;

			m_rcPart[i] = Calculate_Rect(m_vPartPos[i], float(pTexture->tImgInfo.Width), float(pTexture->tImgInfo.Height), false);

			CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_matWorld[i]);
			CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
				NULL,
				NULL,
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		// Up Button
		const TEXINFO* pTexture
			= CTextureMgr::Get_Instance()->GetTexture(L"Button", L"UpButton", 0);

		if (nullptr == pTexture)
			return;

		m_rcUp = Calculate_Rect(m_vUpPos, float(pTexture->tImgInfo.Width), float(pTexture->tImgInfo.Height));


		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_matUpWorld);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// 닫혀있을 때
	else
	{
		const TEXINFO* pTexture
			= CTextureMgr::Get_Instance()->GetTexture(L"Button", L"DownButton", 0);

		if (nullptr == pTexture)
			return;

		m_rcDown = Calculate_Rect(m_vDownPos, float(pTexture->tImgInfo.Width), float(pTexture->tImgInfo.Height));

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_matDownWorld);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// Rect 디버깅 출력
	for (int i = 0; i < PT_END; ++i)
	{
		CGraphicDev::Get_Instance()->Draw_Rect(m_rcPart[i]);
	}
	CGraphicDev::Get_Instance()->Draw_Rect(m_rcUp);
	CGraphicDev::Get_Instance()->Draw_Rect(m_rcDown);


	m_RenderSort.sort(ComparePriority<CWndUI*>);
	for (auto& pWnd : m_RenderSort)
	{
		pWnd->Render();
	}
}

void CMenuUI::Release(void)
{
	for (int i = 0; i < PT_END; ++i)
	{
		if (m_Wnd[i] != nullptr)
		{
			m_Wnd[i]->Release();
			m_Wnd[i] = nullptr;
		}
	}

	m_RenderSort.clear();
}

void CMenuUI::Update_World_Matrix(void)
{
}

void CMenuUI::Update_State(void)
{
}

void CMenuUI::Reset_Priority(CWndUI * pWnd)
{
	pWnd->Set_Priority(m_iPriority);
	m_iPriority++;
	CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::UI);
}

CWndUI * CMenuUI::Get_ActiveUI(void)
{
	return m_RenderSort.back();
}

void CMenuUI::Show_ShopWnd(void)
{
	//m_Wnd[PT_SHOP]->Show(true);
	//m_Wnd[PT_SHOP]->Set_Priority(m_iPriority);
	//m_iPriority++;
}
