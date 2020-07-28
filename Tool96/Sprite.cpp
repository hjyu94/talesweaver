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

	//# ���� ǥ�� �ؽ���
	if (FAILED(CTextureMgr::Get_Instance()->InsertTexture(L"../Texture/Mouse/X/000%d.png", L"Mouse", TEX_MULTI, NULL, L"X", 12)))
	{
		AfxMessageBox(L"����, ���콺 X �ؽ��� ���� ����");
	}


	return E_NOTIMPL;
}

int CSprite::Progress(void)
{
	return 0;
}

void CSprite::Render(void)
{
	// Texture ǥ��
	if (nullptr != m_pTexture)
	{
		D3DXMATRIX matWorld, matScale, matTrans, matRotZ;

		// �¿� ��Ī����?
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

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);	// ����� �������ִ� �Լ�

		CGraphicDev::Get_Instance()->GetSprite()->Draw(
			m_pTexture->pTexture,
			NULL, // ������ �̹����� ũ��(��Ʈ �ּ�)
			&D3DXVECTOR3(fX, fY, 0.f), // �̹����� ���� ����(���� �ּ�)
			NULL, // �̹����� ��� ��ġ(���� �ּ�)
			D3DCOLOR_ARGB(255, 255, 255, 255)); // ��� ������ ���
	}

	// ���� ǥ��?
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

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);	// ����� �������ִ� �Լ�

		CGraphicDev::Get_Instance()->GetSprite()->Draw(
			pTexture->pTexture,
			NULL, // ������ �̹����� ũ��(��Ʈ �ּ�)
			&D3DXVECTOR3(fX, fY, 0.f), // �̹����� ���� ����(���� �ּ�)
			NULL, // �̹����� ��� ��ġ(���� �ּ�)
			D3DCOLOR_ARGB(255, 255, 255, 255)); // ��� ������ ���
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
