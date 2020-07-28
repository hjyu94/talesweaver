#pragma once

// Tool
#include "Texture_Tool.h"
class CSingleTexture :
	public CTexture
{
public:
	CSingleTexture();
	virtual ~CSingleTexture();

public:
	virtual const TEXINFO* GetTexture(const wstring& wstrStateKey = L"", 
									const int& iCount = 0) override;

	virtual HRESULT InsertTexture(const wstring& wstrFilePath, 
								const D3DCOLOR ColorKey = NULL,
								const wstring& wstrStateKey = L"", 
								const int& iCount = 0) override;

	virtual void Release(void) override;

private:
	TEXINFO*				m_pTexInfo = nullptr;

};

