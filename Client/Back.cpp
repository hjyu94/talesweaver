#include "stdafx.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Back.h"
#include "Player.h"
#include "Item.h"

CBack::CBack()
	: m_iTileX(0)
	, m_iTileY(0)
{
	m_eRenderID = RDR_BACK;
}

CBack::~CBack()
{
	Release();
}

HRESULT CBack::Initialize(void)
{
	return S_OK;
}

int CBack::Progress()
{
	// 바닥에 떨어져있는 아이템 
	for (int i = 0; i < m_vecItem.size(); ++i)
	{
		m_vecItem[i]->Progress();
	}

	for (int i = 0; i < m_vecObj.size(); ++i)
	{
		m_vecObj[i]->Progress();
	}
	return 0;
}

void CBack::Render(void)
{
	D3DXMATRIX	matTrans, matScale, matWorld;

	//##1 맵 출력
	const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(m_tFrame.wstrObjKey);

	if (nullptr == pTexture)
		return;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

	D3DXMatrixTranslation(&matTrans
		, -m_vScroll.x
		, -m_vScroll.y
		, 0.f);

	matWorld = matScale * matTrans;

	CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
		NULL,
		NULL,
		NULL,
		D3DCOLOR_ARGB(255, 255, 255, 255));


	// ##2 타일 출력
	// [치트키] 스페이스바를 누르면 타일이 보임.

	TCHAR		szBuf[MIN_STR] = L"";

	//if (CKeyMgr::Get_Instance()->KeyPressing(VK_SPACE))
	if (GetAsyncKeyState(VK_TAB) & 0x8000)
	{
		for (int i = 0; i < m_vecTile.size(); ++i)
		{
			const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(L"TILE", L"Tile", m_vecTile[i]->eDrawID);

			D3DXMATRIX matWorld, matScale, matTrans, matRotZ;

			D3DXMatrixTranslation(
				&matTrans
				, m_vecTile[i]->vPos.x - m_vScroll.x
				, m_vecTile[i]->vPos.y - m_vScroll.y
				, 0.f);

			float fX = float(pTexture->tImgInfo.Width) / 2.f;
			float fY = float(pTexture->tImgInfo.Height) / 2.f;

			matWorld = matTrans;

			CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);	// 행렬을 곱셈해주는 함수

			CGraphicDev::Get_Instance()->GetSprite()->Draw(
				pTexture->pTexture,
				NULL, // 보여줄 이미지의 크기(렉트 주소)
				&D3DXVECTOR3(fX, fY, 0.f), // 이미지의 센터 지점(벡터 주소)
				NULL, // 이미지의 출력 위치(벡터 주소)
				D3DCOLOR_ARGB(255, 255, 255, 255)); // 모든 색상을 출력

			wsprintf(szBuf, L"%d", i);
			CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(CGraphicDev::Get_Instance()->GetSprite(),
				szBuf,
				lstrlen(szBuf),
				NULL,
				NULL,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	// 바닥에 떨어져있는 아이템 
	for (int i = 0; i < m_vecItem.size(); ++i)
	{
		m_vecItem[i]->Render();
	}

	// 오브젝트 출력
	for (int i = 0; i < m_vecObj.size(); ++i)
	{
		m_vecObj[i]->Render();
		
		//wstring wstrStateKey = m_vecObj[i]->wstrStateKey;
		//int iCnt = m_vecObj[i]->iCount;
		//D3DXVECTOR3	vPos = m_vecObj[i]->vPos;
		//float fScale = m_vecObj[i]->fScale;
		//
		//CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
		//D3DXVECTOR3	vPlayerPos = pPlayer->Get_Info().vPos;
		//
		//RECT rcPlayerView = Calculate_Rect(vPlayerPos, WINCX*2, WINCY*2);
		//POINT ptObject = POINT{ (LONG)vPos.x, (LONG)vPos.y };
		//
		//if (!PtInRect(&rcPlayerView, ptObject))
		//	continue;

		//const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
		//	L"Obj", wstrStateKey, iCnt
		//);

		//float fCX = float(pTexture->tImgInfo.Width) * 0.5f;
		//float fCY = float(pTexture->tImgInfo.Height) * 0.5f;

		//D3DXMATRIX	matTrans;

		//D3DXMatrixTranslation(&matTrans
		//	, vPos.x - m_vScroll.x
		//	, vPos.y - m_vScroll.y
		//	, 0.f);
		//
		//CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);

		//CGraphicDev::Get_Instance()->GetSprite()->Draw(
		//	pTexture->pTexture
		//	, NULL
		//	, &D3DXVECTOR3(fCX, fCY, 0.f)
		//	, NULL
		//	, D3DCOLOR_ARGB(255, 255, 255, 255) // 모든 색상을 출력
		//);
	}
}

void CBack::Release(void)
{
	// 타일 반납
	for (auto& iter = m_vecTile.begin(); iter != m_vecTile.end();)
	{
		if (*iter)
		{
			delete *iter;
			iter = m_vecTile.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	m_vecTile.clear();


	// 아이템
	for (auto& iter = m_vecItem.begin(); iter != m_vecItem.end();)
	{
		if (*iter)
		{
			delete *iter;
			iter = m_vecItem.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	m_vecItem.clear();

	// 오브젝트 반납
	for (auto& iter = m_vecObj.begin(); iter != m_vecObj.end();)
	{
		if (*iter)
		{
			delete *iter;
			iter = m_vecObj.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	m_vecObj.clear();
}

void CBack::Load_File(void)
{
	//# 타일
	{
		wstring wstrFilePath
			= L"../Data/Tile/" + m_tFrame.wstrObjKey + L".dat";

		HANDLE hFile = CreateFile(
			wstrFilePath.c_str()
			, GENERIC_READ
			, 0
			, NULL
			, OPEN_EXISTING
			, FILE_ATTRIBUTE_NORMAL
			, NULL);

		DWORD		dwByte;

		ReadFile(hFile, &m_iTileX, sizeof(int), &dwByte, NULL);
		ReadFile(hFile, &m_iTileY, sizeof(int), &dwByte, NULL);

		int i = 0;

		while (true)
		{

			TILE*		pTile = new TILE;

			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

			if (0 == dwByte)
			{
				Safe_Delete(pTile);
				break;
			}

			pTile->wstrInfo._Alloc_proxy(); // wstring 터짐...?
			m_vecTile.push_back(pTile);

			++i;
		}

		CloseHandle(hFile);
	}

	//# 오브젝트
	{
		wstring wstrFilePath
			= L"../Data/Obj/" + m_tFrame.wstrObjKey + L".dat";

		HANDLE hFile = CreateFile(
			wstrFilePath.c_str()
			, GENERIC_READ
			, 0
			, NULL
			, OPEN_EXISTING
			, FILE_ATTRIBUTE_NORMAL
			, NULL);

		DWORD		dwByte = 0;

		while (true)
		{
			OBJ_DATA	Data = {};
			ReadFile(hFile, &Data, sizeof(OBJ_DATA), &dwByte, NULL);

			if (dwByte == 0)
			{
				break;
			}
			
			Data.wstrObjKey._Alloc_proxy();
			Data.wstrStateKey._Alloc_proxy();

			CEtc* pEtc = new CEtc(Data.wstrStateKey, Data.vPos, Data.iCount);
			pEtc->Initialize();
			m_vecObj.push_back(pEtc);
		}

		CloseHandle(hFile);
	}
}

int CBack::Get_Tile_Index(D3DXVECTOR3 vPos, bool Is_Scroll_Needed)
{
	if (Is_Scroll_Needed)
	{
		vPos.x += m_vScroll.x;
		vPos.y += m_vScroll.y;
	}

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (PickingDot(vPos, i))
		{
			return i;
		}
	}
	return -1;
}

bool CBack::PickingDot(D3DXVECTOR3 vPos, int iIndex)
{
	D3DXVECTOR3			vPoint[4] = {

		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + TILECY / 2.f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX / 2.f, m_vecTile[iIndex]->vPos.y, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - TILECY / 2.f, 0.f),
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX / 2.f, m_vecTile[iIndex]->vPos.y, 0.f)

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

void CBack::Camera_Initialize(void)
{
	auto mapBackInfo = CMapMgr::Get_Instance()->Get_BackInfoPtr();
	auto iter = mapBackInfo->find(m_tFrame.wstrObjKey);
	int iIndex = iter->second->iPlayerIndex;
	D3DXVECTOR3 vCenter = m_vecTile[iIndex]->vPos;
	m_vScroll.x = vCenter.x - WINCX * 0.5f;
	m_vScroll.y = vCenter.y - WINCY * 0.5f;
}

void CBack::Get_Tile_Cnt(int & iTileX, int & iTileY)
{
	iTileX = m_iTileX;
	iTileY = m_iTileY;
}

void CBack::Get_Map_Size(int & iWidth, int & iHeight)
{
	const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(m_tFrame.wstrObjKey);

	iWidth = pTexture->tImgInfo.Width;
	iHeight = pTexture->tImgInfo.Height;
}

void CBack::Get_Option_Draw(D3DXVECTOR3 vPos, TILE_DRAW & eDraw, TILE_OPTION & eOption, bool Is_Scroll_Needed)
{
	if (Is_Scroll_Needed)
	{
		vPos.x += m_vScroll.x;
		vPos.y += m_vScroll.y;
	}

	int iIdx = Get_Tile_Index(vPos);

	if (-1 == iIdx)
		return;

	eDraw = m_vecTile[iIdx]->eDrawID;
	eOption = m_vecTile[iIdx]->eOption;
}

int CBack::Get_Player_Index(wstring wstrStage)
{
	auto mapBackInfo = CMapMgr::Get_Instance()->Get_BackInfoPtr();
	auto iter = mapBackInfo->find(wstrStage);
	return iter->second->iPlayerIndex;
}

void CBack::Late_Progress(void)
{
}

void CBack::Enter_Gate(int iTileIdx, DIR eCurDir)
{
	// 해당 맵에서 플레이어의 위치 저장
	auto mapBackInfo = CMapMgr::Get_Instance()->Get_BackInfoPtr();
	auto iter = mapBackInfo->find(m_tFrame.wstrObjKey);
	BACK_INFO* pInfo = iter->second;
	pInfo->iPlayerIndex = iTileIdx;
	//pInfo->ePlayerDir = eCurDir;

	// 타일에 있는 정보를 읽어와서 그 스테이지로 이동시키기
	m_tFrame.wstrObjKey = m_vecTile[iTileIdx]->wstrInfo;

	// 현재 타일, 오브젝트 반납
	Release();

	// 새로운 스테이지의 타일, 오브젝트 저장
	Load_File();

	iter = mapBackInfo->find(m_tFrame.wstrObjKey);
	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	pPlayer->Set_Pos(iter->second->iPlayerIndex);
	pPlayer->Set_Direction(iter->second->ePlayerDir);
}

void CBack::Delete_Item(CItem * pItem)
{
	for (auto iter = m_vecItem.begin(); iter != m_vecItem.end(); )
	{
		if (*iter == pItem)
		{
			*iter = nullptr;
			iter = m_vecItem.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
