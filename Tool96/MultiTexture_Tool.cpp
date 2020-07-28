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
												0, // �̹��� �о���̴� ���, �⺻���� 0
												pTexInfo->tImgInfo.Format,
												D3DPOOL_MANAGED, // �߿���!!!!!!!!!!!!!!!
												D3DX_DEFAULT, // �ؽ����� �׵θ� ǥ���� ��� �� ���ΰ�
												D3DX_DEFAULT, // �ؽ��ĸ� Ȯ��, ����ϰ� �Ǿ����� ��� ó���� ���ΰ�	
												ColorKey,			// ������ �̹����� ���� ��
												&pTexInfo->tImgInfo,
												NULL,  // 8��Ʈ �̸��� �̹����� ��� �� ���ΰ�
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
