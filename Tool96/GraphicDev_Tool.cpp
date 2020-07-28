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
		//MessageBox(g_hWnd, L"��ġ ���� ����", L"System Error", NULL);
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
		AfxMessageBox(L"����̽� ��ġ ���� ����");
		return E_FAIL;
	}

	// ��������Ʈ ��ü ����

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		AfxMessageBox(L"��������Ʈ ��ü ���� ����");
		return E_FAIL;
	}

	// ��Ʈ ��ü ����

	D3DXFONT_DESCW				tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 15;
	tFontInfo.Width = 8;
	tFontInfo.Weight = FW_THIN;
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"����");

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &tFontInfo, &m_pNormal12)))
	{
		AfxMessageBox(L"��Ʈ ���� ����");
		return E_FAIL;
	}

	// ���� ��ü
	if (FAILED(D3DXCreateLine(m_pGraphicDev, &m_pLine)))
	{
		ERR_MSG(L"���� ���� ����");
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

	d3dpp.Windowed = TRUE;		// ��ü ȭ�� or â ���(false�� ��� ��ü ȭ��)

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
						D3DCOLOR_ARGB(255, 0, 0, 255),	// ����� ����
						1.f,		// z������ �ʱ�ȭ ��
						0);			// ���ٽ� ������ �ʱ�ȭ ��

	m_pGraphicDev->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}
void CGraphicDev::Render_End(HWND hWnd)
{
	m_pSprite->End();
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, hWnd, NULL);

	// 1, 2���� : ��Ʈ �� ���� �ּ� ��, �� ���� �������� �̿��Ͽ� Ȯ��, ����Ͽ� ����� ����
	// EX : 1���� ���� ��Ʈ, 2���� ��������� ū ��Ʈ�� ������ Ȯ���ϸ鼭 ȭ�� ���
	// 3���� : ��� �����쿡 �̰��� ������ ���ΰ�(�ڵ� ��)
	// 4���� : �������� ���� ���� �����ϱ� ���� ��Ʈ �ּ�

}