#include "stdafx.h"
#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Back.h"
#include "Player.h"
#include "Item.h"

CBack::CBack()
	: m_iMap_Height(0)
	, m_iMap_Width(0)
	, m_iTileX(0)
	, m_iTileY(0)
{
	m_eRenderID = RDR_BACK;
}

CBack::~CBack()
{
	Release();

	// Back 맵
	// for_each(m_mapBack.begin(), m_mapBack.end(), DeleteMap());
	for (auto& iter = m_mapBack.begin(); iter != m_mapBack.end(); )
	{
		if (iter->second)
		{
			delete iter->second;
			iter->second = nullptr;
			iter = m_mapBack.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	m_mapBack.clear();
}

HRESULT CBack::Initialize(void)
{
	wstring wstrStage = L"Town1";
	BACK_INFO* pBack = new BACK_INFO;
	pBack->ePlayerDir = D_LD;
	pBack->iPlayerIndex = 531;
	m_mapBack.emplace(wstrStage, pBack);

	wstrStage = L"Store0";
	pBack = new BACK_INFO;
	pBack->ePlayerDir = D_LU;
	pBack->iPlayerIndex = 471;
	m_mapBack.emplace(wstrStage, pBack);

	wstrStage = L"Store1";
	pBack = new BACK_INFO;
	pBack->ePlayerDir = D_LU;
	pBack->iPlayerIndex = 307;
	m_mapBack.emplace(wstrStage, pBack);

	wstrStage = L"Store2";
	pBack = new BACK_INFO;
	pBack->ePlayerDir = D_LU;
	pBack->iPlayerIndex = 414;
	m_mapBack.emplace(wstrStage, pBack);

	wstrStage = L"Field0";
	pBack = new BACK_INFO;
	pBack->ePlayerDir = D_LD;
	pBack->iPlayerIndex = 1000;
	m_mapBack.emplace(wstrStage, pBack);


	m_tFrame.wstrObjKey = L"Town1";
	Load_Tile();
	

	return S_OK;
}

int CBack::Progress()
{
	// 바닥에 떨어져있는 아이템 
	for (int i = 0; i < m_vecItem.size(); ++i)
	{
		m_vecItem[i]->Progress();
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

	m_iMap_Height = float(pTexture->tImgInfo.Height);
	m_iMap_Width = float(pTexture->tImgInfo.Width);

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
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
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
			CGraphicDev::Get_Instance()->GetFont()->DrawTextW(CGraphicDev::Get_Instance()->GetSprite(),
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
}

void CBack::Load_Tile(void)
{
	// 데이터에서 타일 정보 읽어와서 벡터에 추가하기
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
	wchar_t		strStageName[MIN_STR];

	ReadFile(hFile, &m_iTileX, sizeof(int), &dwByte, NULL);
	ReadFile(hFile, &m_iTileY, sizeof(int), &dwByte, NULL);

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
	}

	CloseHandle(hFile);
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

void CBack::Get_Tile_Cnt(int & iTileX, int & iTileY)
{
	iTileX = m_iTileX;
	iTileY = m_iTileY;
}

void CBack::Get_Map_Size(int & iWidth, int & iHeight)
{
	iWidth = m_iMap_Width;
	iHeight = m_iMap_Height;
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
	auto iter = m_mapBack.find(wstrStage);
	return iter->second->iPlayerIndex;
}

void CBack::Late_Progress(void)
{
}

void CBack::Enter_Gate(int iTileIdx, DIR eCurDir)
{
	// 해당 맵에서 플레이어의 위치 저장
	auto iter = m_mapBack.find(m_tFrame.wstrObjKey);
	BACK_INFO* pInfo = iter->second;
	pInfo->iPlayerIndex = iTileIdx;
	//pInfo->ePlayerDir = eCurDir;

	// 타일에 있는 정보를 읽어와서 그 스테이지로 이동시키기
	m_tFrame.wstrObjKey = m_vecTile[iTileIdx]->wstrInfo;

	// 현재 타일, 오브젝트 반납
	Release();

	// 새로운 스테이지의 타일, 오브젝트 저장
	Load_Tile();
	
	iter = m_mapBack.find(m_tFrame.wstrObjKey);
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

void CBack::Update_World_Matrix(void)
{
}

void CBack::Update_State(void)
{
}