#pragma once

#include "Include.h"

class CGraphicDev
{
	DECLARE_SINGLETON(CGraphicDev)

public:
	LPDIRECT3DDEVICE9		GetDevice(void) { return m_pGraphicDev; }
	LPD3DXSPRITE			GetSprite(void) { return m_pSprite; }
	LPD3DXFONT				GetFont_Normal12(void)	{ return m_pNormal12; }
	LPD3DXFONT				GetFont_Bold15(void) { return m_pBold15; }
	LPD3DXFONT				GetFont_Heavy15(void)	{ return m_pHeavy15; }
	LPD3DXLINE				GetLine(void) { return m_pLine; }

private:
	LPDIRECT3D9				m_pSDK;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPD3DXSPRITE			m_pSprite;
	LPD3DXFONT				m_pNormal12;
	LPD3DXFONT				m_pBold15;
	LPD3DXFONT				m_pHeavy15;
	LPD3DXLINE				m_pLine;

public:
	HRESULT			Initialize(void);
	void			Release(void);
	void			Render_Begin(void);
	void			Render_End(HWND hWnd = NULL);
	
public:
	void			Draw_Rect(RECT tRect);

private:
	void			SetParameters(D3DPRESENT_PARAMETERS& d3dpp);

private:
	CGraphicDev();
	~CGraphicDev();
};

