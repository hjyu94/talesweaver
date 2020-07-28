#include "stdafx.h"
#include "HJ_Map.h"

#include "TextureMgr_Tool.h"
#include "GraphicDev_Tool.h"
#include "Tool96View.h"
#include "MapTab.h"
#include "MainFrm.h"
#include "Obj.h"
#include "Tool96View.h"

CHJ_Map::CHJ_Map()
	: m_pMainView(nullptr)
	, m_cstrStage(L"")
	, m_fScale(1.f)
	, m_bPrev(false)
{
}

CHJ_Map::~CHJ_Map()
{
	Safe_Delete(m_pPrevObj);
	Release();
}

HRESULT CHJ_Map::Initialize(void)
{
	m_pMapTab = ((CMainFrame*)AfxGetMainWnd())->m_pFormView->m_pMapDialog;
	m_pMapTab->Set_pHJ_Map(this);

	m_pUnitTab = ((CMainFrame*)AfxGetMainWnd())->m_pFormView->m_pUnitTab;
	m_pUnitTab->Set_Map(this);

	m_pPrevObj = new CObj;
	OBJ_DATA tData;
	tData.fScale = 1.f;
	tData.iAlpha = 0.f;
	tData.wstrObjKey = L"Obj";
	m_pPrevObj->Set_Data(tData);

	return S_OK;
}

int CHJ_Map::Progress(void)
{
	return 0;
}

void CHJ_Map::Render(void)
{
	CGraphicDev::Get_Instance()->Render_Begin();
	Stage_Render();
	Tile_Render();
	Obj_Render();
	CGraphicDev::Get_Instance()->Render_End();
}

void CHJ_Map::Stage_Render(void)
{
	//##1 통맵 렌더링
	D3DXMATRIX	matTrans, matScale, matWorld;

	if (L"" != m_cstrStage)
	{
		CString cstrObjKey = L"HJ_";
		CString cstrStateKey = L"";
		CString cstrTemp = L"";
		int		i = 0;

		for (i = 0; i < m_cstrStage.GetLength(); ++i)
		{
			if (0 != isdigit(m_cstrStage[i])) // 숫자인 경우
			{
				break;
			}
			else // 문자인 경우
			{
				cstrStateKey += m_cstrStage[i];
			}
		}

		cstrTemp = cstrStateKey;
		cstrObjKey += cstrTemp.MakeUpper();

		cstrTemp = m_cstrStage;
		cstrTemp.Delete(0, i);
		int iCnt = _tstoi(cstrTemp);

		const TEXINFO*		pTexture = CTextureMgr::Get_Instance()->GetTexture(
			cstrObjKey.operator LPCWSTR(), cstrStateKey.operator LPCWSTR(), iCnt
		);

		if (nullptr != m_pMapTab)
		{
			m_pMapTab->Set_Map_Size_Text(pTexture->tImgInfo.Width, pTexture->tImgInfo.Height);
			m_iStageHeight = pTexture->tImgInfo.Height;
			m_iStageWidth = pTexture->tImgInfo.Width;
			m_pMainView->SetScrollSizes(MM_TEXT, CSize(m_iStageWidth*m_fScale, m_iStageHeight*m_fScale));
		}

		D3DXMatrixScaling(&matScale, m_fScale, m_fScale, 0.f);

		D3DXMatrixTranslation(&matTrans,
			-m_pMainView->GetScrollPos(0),
			-m_pMainView->GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}


void CHJ_Map::Tile_Render(void)
{
	//##2 타일 렌더링
	D3DXMATRIX	matTrans, matScale, matWorld;

	TCHAR		szBuf[MIN_STR] = L"";
	if (!m_vecTile.empty())
	{
		for (int i = 0; i < m_iTileY; ++i)
		{
			for (int j = 0; j < m_iTileX; ++j)
			{
				int	iIndex = i * m_iTileX + j;

				const TEXINFO*		pTexture = CTextureMgr::Get_Instance()->GetTexture(L"HJ_TILE", L"Tile", m_vecTile[iIndex]->eDrawID);

				D3DXMatrixTranslation(&matTrans,
					m_vecTile[iIndex]->vPos.x * m_fScale - m_pMainView->GetScrollPos(0),
					m_vecTile[iIndex]->vPos.y * m_fScale - m_pMainView->GetScrollPos(1),
					0.f);

				D3DXMatrixScaling(&matScale, m_fScale, m_fScale, 0.f);

				matWorld = matScale * matTrans;

				int iTileCX = pTexture->tImgInfo.Width;
				int iTileCY = pTexture->tImgInfo.Height;

				CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);
				CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
					NULL,
					&D3DXVECTOR3(iTileCX * 0.5f, iTileCY * 0.5f, 0.f),
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));

				wsprintf(szBuf, L"%d", iIndex);
				CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
					CGraphicDev::Get_Instance()->GetSprite(),
					szBuf,
					lstrlen(szBuf),
					NULL,
					NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}
}

void CHJ_Map::Obj_Render(void)
{
	//#3 오브젝트 렌더링
	D3DXMATRIX	matTrans, matScale, matWorld;

	TCHAR szBuf[MIN_STR] = L"";

	// 맵탭에서 오브젝트 설정 중일 때
	if (m_bPrev && Get_pMainView()->Get_eTab() == TAB_MAP)
	{
		m_pPrevObj->Set_Alpha(100);
		m_pPrevObj->Render(m_pMapTab->Get_MapScale());
	}

	for (CObj* pObj : m_vecObject)
	{
		//pObj->Render();
		pObj->Render(m_fScale);
	}
}

void CHJ_Map::Release(void)
{
	//for_each(m_vecTile.begin(), m_vecTile.end(), DeleteObj());
	// 질문. 왜 터짐....?
	m_vecTile.clear();

	// 확인, 질문
	//for_each(m_vecObject.begin(), m_vecObject.end(), DeleteObj());
	for (auto iter = m_vecObject.begin()
		; iter != m_vecObject.end()
		;)
	{
		iter = m_vecObject.erase(iter);
	}
	m_vecObject.clear();
}

HRESULT CHJ_Map::Tile_Initialize()
{
	Release();

	m_iTileX = m_iStageWidth / TILECX + 1;
	m_iTileY = m_iStageHeight / (TILECY * 0.5f) + 1;

	for (int i = 0; i < m_iTileY; ++i)
	{
		for (int j = 0; j < m_iTileX; ++j)
		{
			TILE*		pTile = new TILE;

			float	fX = (j * TILECX) + ((i % 2) * (TILECX / 2.f));
			float	fY = i * (TILECY / 2.f);

			pTile->vPos = D3DXVECTOR3(fX, fY, 0.f);
			pTile->vSize = D3DXVECTOR3((float)TILECX, (float)TILECY, 0.f);
			pTile->eOption = TO_MOVE;
			pTile->eDrawID = TD_NORMAL;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

void CHJ_Map::Reset_Tile()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), DeleteObj());
	m_vecTile.clear();
}

void CHJ_Map::Create_Object(CPoint& point, OBJ_DATA tData)
{
	//CObj* pObj = new CObj(tData);

	point.x *= m_fScale;
	point.y *= m_fScale;

	CObj* pObj = new CObj;
	pObj->Set_Data(tData);
	pObj->Initialize();
	pObj->Set_Center(point);
	m_vecObject.push_back(pObj);
}

void CHJ_Map::Reset_Object()
{
	for_each(m_vecObject.begin(), m_vecObject.end(), DeleteObj());
	m_vecObject.clear();
}

void CHJ_Map::Load_File()
{
	//##1) 맵
	{
		CString cstrPath = L"../Data/Tile/" + m_cstrStage + L".dat";

		HANDLE	hFile = CreateFile(
			cstrPath,
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		for_each(m_vecTile.begin(), m_vecTile.end(), DeleteObj());
		m_vecObject.clear();

		DWORD		dwByte;
		wchar_t		strStageName[MIN_STR];
		int				iTileX, iTileY;

		ReadFile(hFile, &iTileX, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &iTileY, sizeof(int), &dwByte, NULL);

		m_iTileX = iTileX;
		m_iTileY = iTileY;

		while (true)
		{
			TILE*		pTile = new TILE;

			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

			if (0 == dwByte)
			{
				Safe_Delete(pTile);
				break;
			}

			m_vecTile.push_back(pTile);
		}

		CloseHandle(hFile);
	}

	//##2 오브젝트
	{
		CString cstrPath = L"../Data/Obj/" + m_cstrStage + L".dat";

		HANDLE hFile = CreateFile(
			cstrPath
			, GENERIC_READ
			, 0
			, NULL
			, OPEN_EXISTING
			, FILE_ATTRIBUTE_NORMAL
			, NULL);

		DWORD		dwByte = 0;

		while (true)
		{
			CObj* pObj = new CObj;
			OBJ_DATA* pData = pObj->Get_DataPtr();
			ReadFile(hFile, pData, sizeof(OBJ_DATA), &dwByte, NULL);

			if (dwByte == 0)
			{
				Safe_Delete(pObj);
				break;
			}

			pData->wstrObjKey._Alloc_proxy();
			pData->wstrObjKey = L"Obj";
			pData->wstrStateKey._Alloc_proxy();

			m_vecObject.push_back(pObj);
		}

		CloseHandle(hFile);
	}
}

void CHJ_Map::Get_StageInfo(int& iTileX, int& iTileY)
{
	iTileX = m_iTileX;
	iTileY = m_iTileY;
}

void CHJ_Map::Tile_Change(const D3DXVECTOR3 & vPos, const TILE_DRAW & eDrawID, const wstring& wstrInfo)
{
	int		iIndex = Get_TileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->eDrawID = eDrawID;

	switch (eDrawID)
	{
	case TD_NORMAL:
	case TD_TRANSPARENT:
	case TD_GATE:
		m_vecTile[iIndex]->eOption = TO_MOVE;
		break;

	case TD_NO_MOVE:
		m_vecTile[iIndex]->eOption = TO_NO_MOVE;
		break;
	}

	if (wstrInfo != L"")
		m_vecTile[iIndex]->wstrInfo = wstrInfo;
}

int CHJ_Map::Get_TileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (PickingDot(vPos, i))
		{
			return i;
		}
	}

	return -1;
}

bool CHJ_Map::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	return false;
}

bool CHJ_Map::PickingDot(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	D3DXVECTOR3			vPoint[4] = {

		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x*m_fScale, m_vecTile[iIndex]->vPos.y*m_fScale + TILECY*m_fScale / 2.f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x*m_fScale + TILECX*m_fScale / 2.f, m_vecTile[iIndex]->vPos.y*m_fScale, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x*m_fScale, m_vecTile[iIndex]->vPos.y*m_fScale - TILECY*m_fScale / 2.f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x*m_fScale - TILECX*m_fScale / 2.f, m_vecTile[iIndex]->vPos.y*m_fScale, 0.f)

	};


	D3DXVECTOR3			vDirection[4] = {

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]

	};


	D3DXVECTOR3			vNormal[4] = {

		D3DXVECTOR3(-vDirection[0].y, vDirection[0].x, 0.f),
		D3DXVECTOR3(-vDirection[1].y, vDirection[1].x, 0.f),
		D3DXVECTOR3(-vDirection[2].y, vDirection[2].x, 0.f),
		D3DXVECTOR3(-vDirection[3].y, vDirection[3].x, 0.f)
	};



	for (size_t i = 0; i < 4; ++i)
	{
		D3DXVECTOR3	vDestDir = vPos - vPoint[i];
		D3DXVec3Normalize(&vDestDir, &vDestDir);
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

		float fDot = D3DXVec3Dot(&vDestDir, &vNormal[i]);

		if (fDot > 0.f)
			return false;
	}

	return true;
}
