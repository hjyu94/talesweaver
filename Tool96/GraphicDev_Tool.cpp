#include "stdafx.h"
#include "GraphicDev_Tool.h"

// Tool
IMPLEMENT_SINGLETON(CGraphicDev)



CGraphicDev::CGraphicDev()
	: m_pSDK(NULL)
	, m_pGraphicDev(NULL)
	, m_pSprite(NULL)
{
}


CGraphicDev::~CGraphicDev()
{
	Release();
}

HRESULT CGraphicDev::Initialize(void)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		//AfxMessageBox();
		//MessageBox(g_hWnd, L"장치 생성 실패", L"System Error", NULL);
		return E_FAIL;
	}

	DWORD		vp;
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp);

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									g_hWnd,
									vp,
									&d3dpp,
									&m_pGraphicDev)))
	{
		AfxMessageBox(L"디바이스 장치 생성 실패");
		return E_FAIL;
	}

	// 스프라이트 객체 생성

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		AfxMessageBox(L"스프라이트 객체 생성 실패");
		return E_FAIL;
	}

	// 폰트 객체 생성

	D3DXFONT_DESCW				tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 15;
	tFontInfo.Width = 8;
	tFontInfo.Weight = FW_THIN;
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"돋움");

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &tFontInfo, &m_pNormal12)))
	{
		AfxMessageBox(L"폰트 생성 실패");
		return E_FAIL;
	}

	// 라인 객체
	if (FAILED(D3DXCreateLine(m_pGraphicDev, &m_pLine)))
	{
		ERR_MSG(L"라인 생성 실패");
		return E_FAIL;
	}

	return S_OK;
}

void CGraphicDev::Release(void)
{
	m_pNormal12->Release();
	m_pSprite->Release();
	m_pGraphicDev->Release();
	m_pSDK->Release();
}

void CGraphicDev::SetParameters(D3DPRESENT_PARAMETERS& d3dpp)
{
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWnd;

	d3dpp.Windowed = TRUE;		// 전체 화면 or 창 모드(false인 경우 전체 화면)

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
void CGraphicDev::Render_Begin(void)
{
	m_pGraphicDev->Clear(0, 
						NULL,
						D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 
						D3DCOLOR_ARGB(255, 0, 0, 255),	// 백버퍼 색상
						1.f,		// z버퍼의 초기화 값
						0);			// 스텐실 버퍼의 초기화 값

	m_pGraphicDev->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}
void CGraphicDev::Render_End(HWND hWnd)
{
	m_pSprite->End();
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, hWnd, NULL);

	// 1, 2인자 : 렉트 두 개의 주소 값, 두 개의 비율차를 이용하여 확대, 축소하여 출력이 가능
	// EX : 1번에 작은 렉트, 2번에 상대적으로 큰 렉트를 넣으면 확대하면서 화면 출력
	// 3인자 : 어느 윈도우에 이것을 적용할 것인가(핸들 값)
	// 4인자 : 보여주지 않을 곳을 설정하기 위한 렉트 주소

}