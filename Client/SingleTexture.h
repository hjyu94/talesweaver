#pragma once
#include "Texture.h"
class CSingleTexture :
	public CTexture
{
public:
	CSingleTexture();
	virtual ~CSingleTexture();

public:
	virtual const TEXINFO* GetTexture(const wstring& wstrStateKey = L"", 
									const int& iCount = 0) override;

	virtual HRESULT InsertTexture(
		const wstring & wstrFilePath
		, SPRITE_INFO tSpriteInfo
		, const wstring & wstrStateKey = L""
		, const int & iCount = 0) override;

	virtual void Release(void) override;

private:
	TEXINFO*				m_pTexInfo = nullptr;



};

