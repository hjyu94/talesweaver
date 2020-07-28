#pragma once

#include "Include.h"

class CTexture
{
public:
	virtual const TEXINFO*		GetTexture(const wstring& wstrStateKey = L"", 
											const int& iCount = 0)PURE;

	virtual HRESULT		InsertTexture(const wstring& wstrFilePath, 
										SPRITE_INFO tSpriteInfo,
										const wstring& wstrStateKey = L"", 
										const int& iCount = 0)PURE;

	virtual void Release(void)PURE;


public:
	CTexture();
	virtual ~CTexture();
};

