#pragma once
#include "Texture.h"
class CMultiTexture :
	public CTexture
{
public:
	CMultiTexture();
	virtual ~CMultiTexture();
public:
	virtual const TEXINFO* GetTexture(const wstring& wstrStateKey = L"",
									  const int& iCount = 0) override;


	virtual HRESULT InsertTexture(
		const wstring & wstrFilePath
		, SPRITE_INFO tSpriteInfo
		, const wstring & wstrStateKey = L""
		, const int & iCount = 0) override;

	//virtual HRESULT InsertTexture(const wstring& wstrFilePath,
	//							const D3DCOLOR ColorKey = NULL,
	//							const wstring& wstrStateKey = L"",
	//							const int& iCount = 0) override;

	virtual void Release(void) override;

public:
	int		Get_SpriteCnt(const wstring& wstrStateKey);

private:
	map<wstring, vector<TEXINFO*>>		m_mapMultiTexture;

};

