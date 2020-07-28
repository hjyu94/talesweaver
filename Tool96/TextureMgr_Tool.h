#pragma once

// Tool

#include "Include.h"

#include "SingleTexture_Tool.h"
#include "MultiTexture_Tool.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO* GetTexture(const wstring& wstrObjKey,
							const wstring& wstrStateKey = L"",
							const int& iCount = 0) ;

public:
	HRESULT		InsertTexture(const wstring& wstrFilePath, 
								const wstring& wstrObjKey, 
								TEXTYPE eType, 
								const D3DCOLOR tColorKey = NULL,
								const wstring& wstrStateKey = L"",
								const int& iCnt = 0);

	void		Release(void);

private:
	map<wstring, CTexture*>				m_MapTexture;

};

