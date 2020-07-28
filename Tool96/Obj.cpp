#include "stdafx.h"
#include "Obj.h"

#include "TextureMgr_Tool.h"
#include "GraphicDev_Tool.h"
#include "Tool96View.h"
#include "MainFrm.h"

CObj::CObj()
{
	ZeroMemory(&m_tData, sizeof(OBJ_DATA));
	m_tData.m_fRadius = 5.f;
}

CObj::~CObj()
{
	Release();
}

HRESULT CObj::Initialize(void)
{
	m_tData.wstrObjKey = L"Obj";
	m_tData.m_fRadius = 5.f;
	return S_OK;
}

int CObj::Progress(void)
{
	return 0;
}

//void CObj::Render(void)
//{
//	D3DXMATRIX matScale, matTrans, matWorld;
//		const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
//		m_tData.wstrObjKey, m_tData.wstrStateKey, m_tData.iCount
//	);
//	
//	if (nullptr == pTexture)
//		return;
//
//	D3DXMatrixScaling(&matScale, m_tData.fScale, m_tData.fScale, 0.f);
//	D3DXMatrixTranslation(
//		&matTrans
//		, m_tData.vPos.x - ((CMainFrame*)AfxGetMainWnd())->m_pMainView->GetScrollPos(0)
//		, m_tData.vPos.y - ((CMainFrame*)AfxGetMainWnd())->m_pMainView->GetScrollPos(1)
//		, 0.f);
//	
//	matWorld = matScale * matTrans;
//
//	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);
//	CGraphicDev::Get_Instance()->GetSprite()->Draw(
//		pTexture->pTexture
//		, NULL
//		, &D3DXVECTOR3(pTexture->tImgInfo.Width / 2.f, pTexture->tImgInfo.Height / 2.f, 0.f)
//		, NULL
//		, D3DCOLOR_ARGB(m_tData.iAlpha, 255, 255, 255)
//	);
//
//
//	//D3DXVECTOR2			vPoint[5] = {
//
//	//	D3DXVECTOR2(m_vCenter.x - m_tData.m_fRadius, m_vCenter.y - m_tData.m_fRadius),
//	//	D3DXVECTOR2(m_vCenter.x + m_tData.m_fRadius, m_vCenter.y - m_tData.m_fRadius),
//	//	D3DXVECTOR2(m_vCenter.x + m_tData.m_fRadius, m_vCenter.y + m_tData.m_fRadius),
//	//	D3DXVECTOR2(m_vCenter.x - m_tData.m_fRadius, m_vCenter.y + m_tData.m_fRadius),
//	//	D3DXVECTOR2(m_vCenter.x - m_tData.m_fRadius, m_vCenter.y - m_tData.m_fRadius),
//
//	//};
//
//	//CGraphicDev::Get_Instance()->Render_End();
//	//CGraphicDev::Get_Instance()->Render_Begin();
//
//	//CGraphicDev::Get_Instance()->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
//
//	//CGraphicDev::Get_Instance()->Render_End();
//	//CGraphicDev::Get_Instance()->Render_Begin();
//}

void CObj::Render(float fScale)
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
		L"Obj"/*m_tData.wstrObjKey*/, m_tData.wstrStateKey, m_tData.iCount
	);

	if (nullptr == pTexture)
		return;

	D3DXMatrixScaling(&matScale, m_tData.fScale*fScale, m_tData.fScale*fScale, 0.f);
	D3DXMatrixTranslation(
		&matTrans
		, (m_tData.vPos.x - ((CMainFrame*)AfxGetMainWnd())->m_pMainView->GetScrollPos(0))*fScale
		, (m_tData.vPos.y - ((CMainFrame*)AfxGetMainWnd())->m_pMainView->GetScrollPos(1))*fScale
		, 0.f);

	matWorld = matScale * matTrans;

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(
		pTexture->pTexture
		, NULL
		, &D3DXVECTOR3(pTexture->tImgInfo.Width / 2.f* fScale, pTexture->tImgInfo.Height / 2.f * fScale, 0.f)
		, NULL
		, D3DCOLOR_ARGB(m_tData.iAlpha, 255, 255, 255)
	);

	TCHAR szBuf[MIN_STR] = L"¡Ú";
	CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
		CGraphicDev::Get_Instance()->GetSprite(),
		szBuf,
		lstrlen(szBuf),
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));
	/*
	Ellipse(m_hDC
	, m_vCenter.x - m_tData.m_fRadius, m_vCenter.y - m_tData.m_fRadius
	, m_vCenter.x + m_tData.m_fRadius, m_vCenter.y + m_tData.m_fRadius);
	*/

	//	D3DXVECTOR2			vPoint[5] = {
	//
	//		D3DXVECTOR2(m_vCenter.x - m_tData.m_fRadius, m_vCenter.y - m_tData.m_fRadius),
	//		D3DXVECTOR2(m_vCenter.x + m_tData.m_fRadius, m_vCenter.y - m_tData.m_fRadius),
	//		D3DXVECTOR2(m_vCenter.x + m_tData.m_fRadius, m_vCenter.y + m_tData.m_fRadius),
	//		D3DXVECTOR2(m_vCenter.x - m_tData.m_fRadius, m_vCenter.y + m_tData.m_fRadius),
	//		D3DXVECTOR2(m_vCenter.x - m_tData.m_fRadius, m_vCenter.y - m_tData.m_fRadius),
	//
	//	};
	//
	//	/*CGraphicDev::Get_Instance()->Render_End();
	//	CGraphicDev::Get_Instance()->Render_Begin();
	//*/
	//	CGraphicDev::Get_Instance()->GetLine()->Draw(vPoint, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
	//
	//	/*CGraphicDev::Get_Instance()->Render_End();
	//	CGraphicDev::Get_Instance()->Render_Begin();*/
}

void CObj::Release(void)
{
}
