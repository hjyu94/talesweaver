#include "stdafx.h"
#include "MyButton.h"
#include "TextureMgr.h"
#include "GraphicDev.h"
#include "ObjMgr.h"
#include "SceneMgr.h"

CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
	Release();
}

HRESULT CMyButton::Initialize(void)
{

	return E_NOTIMPL;
}

int CMyButton::Progress(void)
{
	// Matrix
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(
		&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f
	);

	m_tInfo.matWorld = matTrans;

	// Rect
	const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, m_tFrame.iSpriteCnt
	);

	if (pTexture == nullptr)
		return 0;

	float fX = float(pTexture->tImgInfo.Width);
	float fY = float(pTexture->tImgInfo.Height);

	m_tRect = Calculate_Rect(m_tInfo.vPos, fX, fY);
	
	//{
	//	(LONG)(m_tInfo.vPos.x - fX*0.5f),
	//	(LONG)(m_tInfo.vPos.y - fY*0.5f),
	//	(LONG)(m_tInfo.vPos.x + fX*0.5f),
	//	(LONG)(m_tInfo.vPos.y + fY*0.5f)
	//};

	// 마우스 인식
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	
	CGraphicDev::Get_Instance()->Draw_Rect(m_tRect);

	if (PtInRect(&m_tRect, pt))
	{
		m_tFrame.iSpriteCnt = 1;
		CMouse::m_bIsOnLogoBtn = true;

		if (!bAlreadySound)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"BtnOver.wav", CSoundMgr::CHANNELID::UI);
			bAlreadySound = true;
		}

		if (CKeyMgr::Get_Instance()->KeyDown(VK_LBUTTON, CKeyMgr::S_UI_MENU))
		{
			if (!lstrcmp(m_tFrame.wstrObjKey.c_str(), L"ExitBtn"))
			{
				CSoundMgr::Get_Instance()->PlaySound(L"BtnClick.wav", CSoundMgr::CHANNELID::UI);
				DestroyWindow(g_hWnd);
			}
			else if (!lstrcmp(m_tFrame.wstrObjKey.c_str(), L"StartBtn"))
			{
				if (CTextureMgr::Get_Instance()->Is_Load_Completed())
				{
					CSoundMgr::Get_Instance()->PlaySound(L"BtnClick.wav", CSoundMgr::CHANNELID::UI);
					CSceneMgr::Get_Instance()->SetScene(SC_TOWN1);
				}
			}
		}
	}
	else
	{
		m_tFrame.iSpriteCnt = 0;
		bAlreadySound = false;
	}
			


	return 0;
}

void CMyButton::Render(void)
{
	const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, m_tFrame.iSpriteCnt
	);

	if (nullptr == pTexture)
		return;

	float fX = float(pTexture->tImgInfo.Width);
	float fY = float(pTexture->tImgInfo.Height);

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	CGraphicDev::Get_Instance()->GetSprite()->Draw(
		pTexture->pTexture
		, NULL
		, NULL
		, NULL
		, D3DCOLOR_ARGB(255, 255, 255, 255) // 모든 색상을 출력
	);

}

void CMyButton::Release(void)
{
}

void CMyButton::Set_ObjKey(wstring wstrObjKey)
{
	m_tFrame.wstrObjKey = wstrObjKey;
	m_tFrame.wstrStateKey = wstrObjKey;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexture(
		m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, 0);

	if (nullptr == pTexInfo)
		return;

	SPRITE_INFO tSprite_Info = pTexInfo->tToolInfo;

	m_tFrame.iSpriteCnt = 0;
	m_tFrame.fFrameTime = 0;
	m_tFrame.fFrameSpeed = tSprite_Info.fFrameSpeed;
	m_tFrame.iSpriteMax = tSprite_Info.iCount;
}

void CMyButton::Set_Pos(D3DXVECTOR3 vPos)
{
	m_tInfo.vPos = vPos;
}

void CMyButton::Update_World_Matrix(void)
{
}

void CMyButton::Update_State(void)
{
}

void CMyButton::Late_Progress(void)
{
}
