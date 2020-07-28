#include "stdafx.h"
#include "GraphicDev.h"

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
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

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
		ERR_MSG(L"����̽� ��ġ ���� ����");
		return E_FAIL;
	}

	// ��������Ʈ ��ü ����

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		ERR_MSG(L"��������Ʈ ��ü ���� ����");
		return E_FAIL;
	}

	// ��Ʈ ��ü ����
	D3DXFONT_DESCW				tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 12;
	tFontInfo.Width = 0;
	tFontInfo.Weight = FW_NORMAL;
	tFontInfo.CharSet = HANGUL_CHARSET;
	//lstrcpy(tFontInfo.FaceName, L"�ü�");
	lstrcpy(tFontInfo.FaceName, L"����");

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &tFontInfo, &m_pNormal12)))
	{
		ERR_MSG(L"��Ʈ ���� ����");
		return E_FAIL;
	}

	// ��Ʈ2 ��ü ����
	D3DXFONT_DESCW				tFontInfo2;
	ZeroMemory(&tFontInfo2, sizeof(D3DXFONT_DESCW));

	tFontInfo2.Height = 15;
	tFontInfo2.Width = 0;
	tFontInfo2.Weight = FW_BOLD;
	tFontInfo2.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo2.FaceName, L"����");

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &tFontInfo2, &m_pBold15)))
	{
		ERR_MSG(L"��Ʈ2 ���� ����");
		return E_FAIL;
	}

	// ���� ��Ʈ ��ü ����
	tFontInfo2;
	ZeroMemory(&tFontInfo2, sizeof(D3DXFONT_DESCW));

	tFontInfo2.Height = 15;
	tFontInfo2.Width = 0;
	tFontInfo2.Weight = FW_HEAVY;
	tFontInfo2.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo2.FaceName, L"����");

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &tFontInfo2, &m_pHeavy15)))
	{
		ERR_MSG(L"��Ʈ2 ���� ����");
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
	m_pBold15->Release();
	m_pLine->Release();
	m_pSprite->Release();
	m_pGraphicDev->Release();
	m_pSDK->Release();
}

void CGraphicDev::Draw_Rect(RECT tRect)
{
	// �����
	if (CKeyMgr::Get_Instance()->KeyPressing(VK_TAB))
	{
		D3DXVECTOR2			vPoint[5] = {

			D3DXVECTOR2(tRect.left, tRect.top),
			D3DXVECTOR2(tRect.right, tRect.top),
			D3DXVECTOR2(tRect.right, tRect.bottom),
			D3DXVECTOR2(tRect.left, tRect.bottom),
			D3DXVECTOR2(tRect.left, tRect.top)

		};

		CGraphicDev::Get_Instance()->Render_End();
		CGraphicDev::Get_Instance()->Render_Begin();

		CGraphicDev::Get_Instance()->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 255, 255, 255));

		CGraphicDev::Get_Instance()->Render_End();
		CGraphicDev::Get_Instance()->Render_Begin();
	}
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

	//d3dpp.Windowed = TRUE;		// ��ü ȭ�� or â ���(false�� ��� ��ü ȭ��)
	d3dpp.Windowed = FALSE;		// ��ü ȭ�� or â ���(false�� ��� ��ü ȭ��)

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
void CGraphicDev::Render_Begin(void)
{
	//m_pGraphicDev->Clear(0, 
	//					NULL,
	//					D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 
	//					D3DCOLOR_ARGB(255, 0, 0, 255),	// ����� ����
	//					1.f,		// z������ �ʱ�ȭ ��
	//					0);			// ���ٽ� ������ �ʱ�ȭ ��

	m_pGraphicDev->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}
void CGraphicDev::Render_End(HWND hWnd)
{
	m_pSprite->End();
	m_pGraphicDev->EndScene();
	//m_pGraphicDev->Present(NULL, NULL, hWnd, NULL);

	// 1, 2���� : ��Ʈ �� ���� �ּ� ��, �� ���� �������� �̿��Ͽ� Ȯ��, ����Ͽ� ����� ����
	// EX : 1���� ���� ��Ʈ, 2���� ��������� ū ��Ʈ�� ������ Ȯ���ϸ鼭 ȭ�� ���
	// 3���� : ��� �����쿡 �̰��� ������ ���ΰ�(�ڵ� ��)
	// 4���� : �������� ���� ���� �����ϱ� ���� ��Ʈ �ּ�

}