// Client
#include "stdafx.h"
#include "TextureMgr.h"
#include <fstream>
#include "MainMenu.h"
#include "SceneMgr.h"

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
									SPRITE_INFO tSpriteInfo,
									const wstring& wstrStateKey /*= L""*/,
									const int& iCnt /*= 0*/)
{
	auto		iter = m_MapTexture.find(wstrObjKey);

	m_wstrPath = wstrFilePath;
	
	if(CSceneMgr::Get_Instance()->Get_SceneID() == SC_MENU)
		CMainMenu::m_iReadLine++;

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

		if (FAILED(pTexture->InsertTexture(wstrFilePath, tSpriteInfo, wstrStateKey, iCnt)))
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
		CMultiTexture* pMultilTex = static_cast<CMultiTexture*>(iter->second);
		pMultilTex->InsertTexture(wstrFilePath, tSpriteInfo, wstrStateKey, iCnt);
	}

	return S_OK;
}

HRESULT CTextureMgr::ReadImgPath(const wstring & wstrImgPath, bool bIsSingleTex)
{
	wifstream		LoadFile;

	LoadFile.open(wstrImgPath, ios::in);

	//# 싱글 텍스쳐의 경우
	if (bIsSingleTex)
	{
		TCHAR		szObjKey[MIN_STR] = L"";
		TCHAR		szImgPath[MAX_PATH] = L"";
		SPRITE_INFO tSprite;

		while (!LoadFile.eof())
		{
			LoadFile.getline(szObjKey, MIN_STR, '|');
			LoadFile.getline(szImgPath, MAX_PATH);

			if (FAILED(InsertTexture(szImgPath, szObjKey, TEX_SINGLE, tSprite)))
			{
				ERR_MSG(szObjKey);
				return E_FAIL;
			}
		}
	}

	//# 멀티 텍스쳐의 경우
	else
	{
		TCHAR		szBuf[MAX_STR] = L"";

		TCHAR		szObjKey[MIN_STR] = L"";
		TCHAR		szStateKey[MIN_STR] = L"";
		TCHAR		szCount[MIN_STR] = L"";
		TCHAR		szImgPath[MAX_PATH] = L"";
		TCHAR		szFrameSpeed[MIN_STR] = L"";
		TCHAR		szIsReversed[MIN_STR] = L"";
		TCHAR		szColorKey[MIN_STR] = L"";
		TCHAR		szCenterPoint[MIN_STR] = L"";

		while (!LoadFile.eof())
		{
			LoadFile.getline(szObjKey, MIN_STR, '|');
			LoadFile.getline(szStateKey, MIN_STR, '|');
			LoadFile.getline(szCount, MIN_STR, '|');
			LoadFile.getline(szImgPath, MAX_PATH, '|');
			LoadFile.getline(szFrameSpeed, MIN_STR, '|');
			LoadFile.getline(szIsReversed, MIN_STR, '|');
			LoadFile.getline(szColorKey, MIN_STR, '|');
			LoadFile.getline(szCenterPoint, MIN_STR);

			if (lstrcmp(szObjKey, L"Anais") == 0)
				int i = 0;

			// 빈칸
			if (lstrcmp(szObjKey, L"") == 0)
				return E_FAIL;

			SPRITE_INFO tSpriteInfo = {};

			int iCnt = _ttoi(szCount);
			int iFrameSpeed = _ttoi(szFrameSpeed);

			tSpriteInfo.iCount = iCnt;

			if (lstrcmp(szIsReversed, L"TRUE") == 0)
			{
				tSpriteInfo.bReverse = true;
			}
			else
			{
				tSpriteInfo.bReverse = false;
			}

			tSpriteInfo.fFrameSpeed = iFrameSpeed*0.001f;

			if (lstrcmp(szColorKey, L"X") == 0)
			{
				tSpriteInfo.tColorKey = NULL;
			}
			else
			{
				wstring wstrColorKey = szColorKey;
				wstring wstrR = L"";
				wstring wstrG = L"";
				wstring wstrB = L"";

				int iCnt = 0;
				int iCutPos_1 = 0;
				int iCutPos_2 = 0;

				for (size_t i = 0; i < wstrColorKey.length(); ++i)
				{
					if (!isdigit(wstrColorKey.at(i)) && iCnt == 0) { iCutPos_1 = i; ++iCnt; }
					else if (!isdigit(wstrColorKey.at(i)) && iCnt == 1) iCutPos_2 = i;
				}

				wstrR = wstrColorKey.substr(0, iCutPos_1);
				wstrG = wstrColorKey.substr(iCutPos_1 + 1, iCutPos_2 - iCutPos_1 - 1);
				wstrB = wstrColorKey.substr(iCutPos_2 + 1, wstrColorKey.length() - iCutPos_2 - 1);

				int iR = _ttoi(wstrR.c_str());
				int iG = _ttoi(wstrG.c_str());
				int iB = _ttoi(wstrB.c_str());

				tSpriteInfo.tColorKey = D3DCOLOR_XRGB(iR, iG, iB);
			}

			if (lstrcmp(szCenterPoint, L"X") == 0)
			{
				tSpriteInfo.ptCenter = { 0,0 };
			}
			else
			{
				wstring wstrCenter = szCenterPoint;
				wstring wstrX = L"";
				wstring wstrY = L"";

				int iCutPos = 0;

				for (size_t i = 0; i < wstrCenter.length(); ++i)
				{
					if (!isdigit(wstrCenter.at(i))) { iCutPos = i; }
				}

				wstrX = wstrCenter.substr(0, iCutPos);
				wstrY = wstrCenter.substr(iCutPos + 1, wstrCenter.length() - iCutPos - 1);

				int iX = _ttoi(wstrX.c_str());
				int iY = _ttoi(wstrY.c_str());
				tSpriteInfo.ptCenter = { iX, iY };
			}

			if (FAILED(InsertTexture(szImgPath, szObjKey, TEX_MULTI, tSpriteInfo, szStateKey, iCnt)))
			{
				ERR_MSG(szObjKey);
				return E_FAIL;
			}

		}
	}

	LoadFile.close();
	return S_OK;	
}

HRESULT CTextureMgr::PersonalProcess(void)
{
	SPRITE_INFO tSprite = {};
	tSprite.tColorKey = D3DCOLOR_XRGB(184, 252, 232);

	wstring wstrFilePath[11];
	wstring wstrObjKey[] = { 
		L"SP100", L"SP80", L"SP10", 
		L"MP80",  L"MP30", L"MP10",
		L"HP100", L"HP80", L"HP10",
		L"RedFoot", L"GreenFoot"
	};

	for (int i = 0; i < 11; ++i)
	{
		wstrFilePath[i] = L"..\\Texture\\Item\\" + wstrObjKey[i] + L".png";
	}

	for (int i = 0; i < 11; ++i)
	{
		CSingleTexture* pTexture = new CSingleTexture;
		pTexture->InsertTexture(wstrFilePath[i], tSprite);

		auto iter = m_MapTexture.find(wstrObjKey[i]);
		if (iter != m_MapTexture.end())
			break;

		m_MapTexture.emplace(wstrObjKey[i], pTexture);
	}


	return S_OK;
}

void CTextureMgr::Release(void)
{
	for (auto& iter = m_MapTexture.begin()
		; iter != m_MapTexture.end()
		; ++iter)
	{
		//iter->second->Release();
		delete iter->second;
	}
	m_MapTexture.clear();

	//for_each(m_MapTexture.begin(), m_MapTexture.end(), DeleteMap());
	//m_MapTexture.clear();
}

size_t CTextureMgr::Get_Texture_Size(const wstring & wstrObjKey, const wstring & wstrStateKey)
{
	auto iter = m_MapTexture.find(wstrObjKey);

	if (iter == m_MapTexture.end())
		return -1;

	CMultiTexture* pTexture = static_cast<CMultiTexture*>(iter->second);

	return pTexture->Get_SpriteCnt(wstrStateKey);
}

