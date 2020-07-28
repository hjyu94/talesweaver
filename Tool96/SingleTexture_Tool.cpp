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
											0, // �̹��� �о���̴� ���, �⺻���� 0
											m_pTexInfo->tImgInfo.Format, 
											D3DPOOL_MANAGED, // �߿���!!!!!!!!!!!!!!!
											D3DX_DEFAULT, // �ؽ����� �׵θ� ǥ���� ��� �� ���ΰ�
											D3DX_DEFAULT, // �ؽ��ĸ� Ȯ��, ����ϰ� �Ǿ����� ��� ó���� ���ΰ�	
											ColorKey,			// ������ �̹����� ���� ��
											&m_pTexInfo->tImgInfo, 
											NULL,  // 8��Ʈ �̸��� �̹����� ��� �� ���ΰ�
											&m_pTexInfo->pTexture )))
	{
		ERR_MSG(L"Single Texture Img Load Failed");
		return E_FAIL;
	}


	// TCHAR		szName[128] = L"ȫ�浿";
	// 1. szName = wstrFilePath;
	// 2. wstrFilePath = szName; // ����
	// 3. lstrcpy(szName, wstrFilePath.c_str());




	return S_OK;
}

void CSingleTexture::Release(void)
{
	m_pTexInfo->pTexture->Release();

	Safe_Delete(m_pTexInfo);
}

