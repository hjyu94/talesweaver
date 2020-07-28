#pragma once
// Client
#include "Include.h"

#include "SingleTexture.h"
#include "MultiTexture.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const wstring&	GetFilePath(void) { return m_wstrPath; }
	void			Set_Name(const wstring& wstrName) { m_wstrPath = wstrName; }

	const TEXINFO*	GetTexture(const wstring& wstrObjKey,
							const wstring& wstrStateKey = L"",
							const int& iCount = 0) ;

public:
	HRESULT			InsertTexture(const wstring& wstrFilePath, 
									const wstring& wstrObjKey, 
									TEXTYPE eType, 
									SPRITE_INFO SpriteInfo,
									const wstring& wstrStateKey = L"",
									const int& iCnt = 0);

	HRESULT			ReadImgPath(const wstring& wstrImgPath, bool bIsSingleTex); // 멀티 텍스쳐용
	HRESULT			PersonalProcess(void);

	void			Release(void);

	size_t			Get_Texture_Size(const wstring& wstrObjKey, const wstring& wstrStateKey);
	void			Complete_Load(bool bIsEnd) { m_bIsLoadEnd = bIsEnd; }
	bool			Is_Load_Completed(void) { return m_bIsLoadEnd; }

private:
	map<wstring, CTexture*>				m_MapTexture;
	wstring								m_wstrPath;
	bool								m_bIsLoadEnd = false;
};

