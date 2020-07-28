#include "stdafx.h"
#include "Sprite.h"

#include "TextureMgr_Tool.h"
#include "GraphicDev_Tool.h"
#include "Tool96View.h"
#include "SpriteTab.h"
#include "MainFrm.h"

CSprite::CSprite()
	: m_pMainView(nullptr)
{
}


CSprite::~CSprite()
{
	Release();
}

HRESULT CSprite::Initialize(void)
{
	m_pSpriteTab = ((CMainFrame*)AfxGetMainWnd())->m_pFormView->m_pSpriteTab;
	m_pSpriteTab->Set_Sprite(this);

	//# 중점 표시 텍스쳐
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Mouse/X/000%d.png", L"Mouse", TEX_MULTI, NULL, L"X", 12)))
	{
		AfxMessageBox(L"중점, 마우스 X 텍스쳐 생성 실패");
	}


	return E_NOTIMPL;
}

int CSprite::Progress(void)
{
	return 0;
}

void CSprite::Render(void)
{
	// Texture 표시
	if (nullptr != m_pTexture)
	{
		D3DXMATRIX matWorld, matScale, matTrans, matRotZ;

		// 좌우 대칭인지?
		if (m_pSpriteTab->Is_Reverse_Btn_Checked())
		{
			D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		}
		else
		{
			D3DXMatrixScaling(&matScale, +1.f, 1.f, 0.f);
		}

		D3DXMatrixTranslation(
			&matTrans
			, 400.f
			, 300.f
			, 0.f);

		float fX = m_pTexture->tImgInfo.Width / 2.f;
		float fY = m_pTexture->tImgInfo.Height / 2.f;

		matWorld = matScale * matTrans;

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);	// 행렬을 곱셈해주는 함수

		CGraphicDev::Get_Instance()->GetSprite()->Draw(
			m_pTexture->pTexture,
			NULL, // 보여줄 이미지의 크기(렉트 주소)
			&D3DXVECTOR3(fX, fY, 0.f), // 이미지의 센터 지점(벡터 주소)
			NULL, // 이미지의 출력 위치(벡터 주소)
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 모든 색상을 출력
	}

	// 중점 표시?
	if (m_pSpriteTab->Is_Middle_Pos_Btn_Checked())
	{
		D3DXMATRIX matWorld, matScale, matTrans, matRotZ;

		const TEXINFO*		pTexture = CTextureMgr::Get_Instance()->GetTexture(
			L"Mouse"
			, L"X"
			, 0
		);

		D3DXMatrixTranslation(
			&matTrans
			, 400.f + m_iMiddleX
			, 300.f + m_iMiddleY
			, 0.f);

		float fX = pTexture->tImgInfo.Width / 2.f ;
		float fY = pTexture->tImgInfo.Height / 2.f;

		matWorld = matTrans;

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);	// 행렬을 곱셈해주는 함수

		CGraphicDev::Get_Instance()->GetSprite()->Draw(
			pTexture->pTexture,
			NULL, // 보여줄 이미지의 크기(렉트 주소)
			&D3DXVECTOR3(fX, fY, 0.f), // 이미지의 센터 지점(벡터 주소)
			NULL, // 이미지의 출력 위치(벡터 주소)
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 모든 색상을 출력
	}
}

void CSprite::Release(void)
{
}

void CSprite::Set_Mid_Point(POINT point)
{
	m_iMiddleX = point.x;
	m_iMiddleY = point.y;
}
