#include "stdafx.h"
#include "MultiTexture_Tool.h"

#include "GraphicDev_Tool.h"

CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO* CMultiTexture::GetTexture(const wstring& wstrStateKey /*= L""*/,
										const int& iCount /*= 0*/)
{
	auto		iter = m_mapMultiTexture.find(wstrStateKey);

	if (iter == m_mapMultiTexture.end())
		return nullptr;
	
	return iter->second[iCount];
}

HRESULT CMultiTexture::InsertTexture(const wstring& wstrFilePath,
								const D3DCOLOR ColorKey /*= NULL*/,
								const wstring& wstrStateKey /*= L""*/,
								const int& iCount /*= 0*/)
{
	TCHAR		szPath[MAX_PATH] = L"";
	
	vector<TEXINFO*>		vecTexture;

	for (int i = 0; i < iCount; ++i)
	{
		wsprintf(szPath, wstrFilePath.c_str(), i);

		TEXINFO*		pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szPath, &pTexInfo->tImgInfo)))
			return E_FAIL;

		if (FAILED(D3DXCreateTextureFromFileEx(CGraphicDev::Get_Instance()->GetDevice(),
												szPath,
												pTexInfo->tImgInfo.Width,
												pTexInfo->tImgInfo.Height,
												pTexInfo->tImgInfo.MipLevels,
												0, // 이미지 읽어들이는 방식, 기본으로 0
												pTexInfo->tImgInfo.Format,
												D3DPOOL_MANAGED, // 중요함!!!!!!!!!!!!!!!
												D3DX_DEFAULT, // 텍스쳐의 테두리 표현을 어떻게 할 것인가
												D3DX_DEFAULT, // 텍스쳐를 확대, 축소하게 되었을떄 어떻게 처리할 것인가	
												ColorKey,			// 제거할 이미지의 색상 값
												&pTexInfo->tImgInfo,
												NULL,  // 8비트 미만의 이미지를 어떻게 할 것인가
												&pTexInfo->pTexture)))
		{
			ERR_MSG(L"Multi Texture Img Load Failed");
			return E_FAIL;
		}

		vecTexture.push_back(pTexInfo);
	}

	m_mapMultiTexture.emplace(wstrStateKey, vecTexture);

	return S_OK;
}

void CMultiTexture::Release(void)
{
	for (auto iter : m_mapMultiTexture)
	{
		for (auto vector : iter.second)
		{
			vector->pTexture->Release();
			Safe_Delete(vector);
		}
		iter.second.clear();
	}
	m_mapMultiTexture.clear();
}
