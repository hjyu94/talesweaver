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


	// �ؽ��� �ʿ� Ű�� ���� ���
	// �̱� �ؽ���, ��Ƽ �ؽ��� �ƹ� ��쳪 ����
	// �̱� �ؽ��Ĵ� ������Ʈ Ű�θ� �����ϰ�,
	// ��Ƽ �ؽ��Ĵ� ������Ʈ Ű��, ������Ʈ Ű �ΰ����� �����Ѵ�.
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

	// �ؽ��� �ʿ� Ű�� �ִ� ���
	// ������ ���� ObjKey�� ���� ��Ƽ �ؽ��ĸ� �־�״� ��� ��.
	// Value�� ��Ƽ�ؽ��� ��ü�� ������ �� ��,
	// �� ��ü�� �ʿ� StateKey, TEXINFO* ���� �־��ָ� �ȴ�.
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
