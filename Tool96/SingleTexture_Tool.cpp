#include "stdafx.h"
#include "SingleTexture_Tool.h"
#include "GraphicDev_Tool.h"

// Tool

CSingleTexture::CSingleTexture()
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}

const TEXINFO* CSingleTexture::GetTexture(const wstring& wstrStateKey /*= L""*/, const int& iCount /*= 0*/)
{
	return m_pTexInfo;
}

HRESULT CSingleTexture::InsertTexture(const wstring& wstrFilePath, 
										const D3DCOLOR ColorKey /*= NULL*/,
										const wstring& wstrStateKey /*= L""*/, 
										const int& iCount /*= 0*/)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	if (FAILED(D3DXGetImageInfoFromFile(wstrFilePath.c_str(), &m_pTexInfo->tImgInfo)))
	{
		ERR_MSG(wstrFilePath.c_str());
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(CGraphicDev::Get_Instance()->GetDevice(),
											wstrFilePath.c_str(), 
											m_pTexInfo->tImgInfo.Width, 
											m_pTexInfo->tImgInfo.Height, 
											m_pTexInfo->tImgInfo.MipLevels, 
											0, // 이미지 읽어들이는 방식, 기본으로 0
											m_pTexInfo->tImgInfo.Format, 
											D3DPOOL_MANAGED, // 중요함!!!!!!!!!!!!!!!
											D3DX_DEFAULT, // 텍스쳐의 테두리 표현을 어떻게 할 것인가
											D3DX_DEFAULT, // 텍스쳐를 확대, 축소하게 되었을떄 어떻게 처리할 것인가	
											ColorKey,			// 제거할 이미지의 색상 값
											&m_pTexInfo->tImgInfo, 
											NULL,  // 8비트 미만의 이미지를 어떻게 할 것인가
											&m_pTexInfo->pTexture )))
	{
		ERR_MSG(L"Single Texture Img Load Failed");
		return E_FAIL;
	}


	// TCHAR		szName[128] = L"홍길동";
	// 1. szName = wstrFilePath;
	// 2. wstrFilePath = szName; // 가능
	// 3. lstrcpy(szName, wstrFilePath.c_str());




	return S_OK;
}

void CSingleTexture::Release(void)
{
	m_pTexInfo->pTexture->Release();

	Safe_Delete(m_pTexInfo);
}

