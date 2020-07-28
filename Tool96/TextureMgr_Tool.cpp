#include "stdafx.h"
#include "TextureMgr_Tool.h"

// Tool

IMPLEMENT_SINGLETON(CTextureMgr)


CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO* CTextureMgr::GetTexture(const wstring& wstrObjKey, 
										const wstring& wstrStateKey /*= L""*/,
										const int& iCount /*= 0*/)
{

	auto		iter = m_MapTexture.find(wstrObjKey);

	if (iter == m_MapTexture.end())
		return nullptr;

	return iter->second->GetTexture(wstrStateKey, iCount);
}

HRESULT CTextureMgr::InsertTexture(const wstring& wstrFilePath,
									const wstring& wstrObjKey,
									TEXTYPE eType,
									D3DCOLOR tColorKey,
									const wstring& wstrStateKey /*= L""*/,
									const int& iCnt /*= 0*/)
{

	auto		iter = m_MapTexture.find(wstrObjKey);


	// 텍스쳐 맵에 키가 없는 경우
	// 싱글 텍스쳐, 멀티 텍스쳐 아무 경우나 가능
	// 싱글 텍스쳐는 오브젝트 키로만 구분하고,
	// 멀티 텍스쳐는 오브젝트 키와, 스테이트 키 두가지로 구분한다.
	if (iter == m_MapTexture.end())
	{
		CTexture*		pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->InsertTexture(wstrFilePath, tColorKey, wstrStateKey, iCnt)))
		{
			ERR_MSG(wstrObjKey.c_str());
			return E_FAIL;
		}

		m_MapTexture.emplace(wstrObjKey, pTexture);
	}

	// 텍스쳐 맵에 키가 있는 경우
	// 이전에 같은 ObjKey를 갖는 멀티 텍스쳐를 넣어뒀던 경우 뿐.
	// Value로 멀티텍스쳐 객체를 가지고 온 뒤,
	// 그 객체의 맵에 StateKey, TEXINFO* 쌍을 넣어주면 된다.
	else
	{
		CMultiTexture* ref_pMulTex = static_cast<CMultiTexture*>(iter->second);
		ref_pMulTex->InsertTexture(wstrFilePath, tColorKey, wstrStateKey, iCnt);
	}

	return S_OK;
}

void CTextureMgr::Release(void)
{
	for_each(m_MapTexture.begin(), m_MapTexture.end(), DeleteMap());
	m_MapTexture.clear();
}
