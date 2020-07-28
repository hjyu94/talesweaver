//#include "stdafx.h"
//#include "Back.h"
//
//#include "TextureMgr_Tool.h"
//#include "GraphicDev_Tool.h"
//#include "Tool96View.h"
//#include "MyForm.h"
//#include "MainFrm.h"
//
//CBack::CBack()
//{
//}
//
//
//CBack::~CBack()
//{
//	Release();
//}
//
//HRESULT CBack::Initialize(void)
//{
//	for (int i = 0; i < TILEY; ++i)
//	{
//		for (int j = 0; j < TILEX; ++j)
//		{
//			TILE*		pTile = new TILE;
//
//			float	fX = j * TILECX + (i % 2) * (TILECX / 2.f);
//			float	fY = i * (TILECY / 2.f);
//
//			pTile->vPos = D3DXVECTOR3(fX, fY, 0.f);
//			pTile->vSize = D3DXVECTOR3((float)TILECX, (float)TILECY, 0.f);
//			pTile->eOption = 0;
//			pTile->eDrawID = 183;
//
//			m_vecTile.push_back(pTile);
//		}
//	}
//
//	//LoadTile();
//
//	return S_OK;
//}
//
//int CBack::Progress(void)
//{
//	return 0;
//}
//
//void CBack::Render(void)
//{
//	TCHAR		szBuf[MIN_STR] = L"";
//
//
//	D3DXMATRIX	matTrans;
//
//	for (int i = 0; i < TILEY; ++i)
//	{
//		for (int j = 0; j < TILEX; ++j)
//		{
//			int	iIndex = i * TILEX + j;
//
//			const TEXINFO*		pTexture = CTextureMgr::Get_Instance()->GetTexture(L"TILE", L"Tile", m_vecTile[iIndex]->eDrawID);
//
//			D3DXMatrixTranslation(&matTrans,
//				m_vecTile[iIndex]->vPos.x - m_pMainView->GetScrollPos(0),
//				m_vecTile[iIndex]->vPos.y - m_pMainView->GetScrollPos(1),
//				0.f);
//
//			CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
//			CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
//				NULL,
//				&D3DXVECTOR3(80.f, 40.f, 0.f),
//				NULL,
//				D3DCOLOR_ARGB(255, 255, 255, 255));
//
//			wsprintf(szBuf, L"%d", iIndex);
//			CGraphicDev::Get_Instance()->GetFont()->DrawTextW(CGraphicDev::Get_Instance()->GetSprite(),
//				szBuf,
//				lstrlen(szBuf),
//				NULL,
//				NULL,
//				D3DCOLOR_ARGB(255, 255, 255, 255));
//		}
//	}
//}
//
//void CBack::MiniRender(void)
//{
//	/*D3DXMATRIX	matWorld, matScale, matTrans;
//
//	for (int i = 0; i < TILEY; ++i)
//	{
//		for (int j = 0; j < TILEX; ++j)
//		{
//			int	iIndex = i * TILEX + j;
//
//			const TEXINFO*		pTexture = CTextureMgr::Get_Instance()->GetTexture(L"TILE", L"Tile", m_vecTile[iIndex]->eDrawID);
//
//			D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0.f);
//			D3DXMatrixTranslation(&matTrans,
//				m_vecTile[iIndex]->vPos.x * 0.3f,
//				m_vecTile[iIndex]->vPos.y * 0.3f,
//				0.f);
//
//			matWorld = matScale * matTrans;
//
//			CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matWorld);
//			CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
//				NULL,
//				&D3DXVECTOR3(65.f, 34.f, 0.f),
//				NULL,
//				D3DCOLOR_ARGB(255, 255, 255, 255));
//		}
//	}*/
//}
//
//void CBack::Release(void)
//{
//	for_each(m_vecTile.begin(), m_vecTile.end(), DeleteObj());
//	m_vecTile.clear();
//}
//
//void CBack::TileChange(const D3DXVECTOR3 & vPos, const int & iDrawID)
//{
//	int		iIndex = GetTileIndex(vPos);
//
//	if (-1 == iIndex)
//		return;
//
//	m_vecTile[iIndex]->eDrawID = iDrawID;
//	m_vecTile[iIndex]->eOption = 1;
//}
//
//int CBack::GetTileIndex(const D3DXVECTOR3 & vPos)
//{
//	for (size_t i = 0; i < m_vecTile.size(); ++i)
//	{
//		if (PickingDot(vPos, i))
//		{
//			return i;
//		}
//	}
//
//	return -1;
//}
//
//bool CBack::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
//{
//	// y = ax + b;
//
//	//float		fGradient = (TILECY / 2.f) / (TILECX / 2.f);
//
//	//D3DXVECTOR3			vPoint[4] = {
//
//	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - TILECY / 2.f, 0.f),
//	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX / 2.f, m_vecTile[iIndex]->vPos.y, 0.f),
//	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + TILECY / 2.f, 0.f),
//	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX / 2.f, m_vecTile[iIndex]->vPos.y, 0.f)
//
//	//};
//
//	//// -b = ax - y => b = -ax + y
//	//float fB[4] = {
//
//	//	-fGradient * vPoint[0].x + vPoint[0].y,
//	//	fGradient * vPoint[1].x + vPoint[1].y,
//	//	-fGradient * vPoint[2].x + vPoint[2].y,
//	//	fGradient * vPoint[3].x + vPoint[3].y
//	//};
//
//	//// y = ax + b =>
//	//// y - ax - b  = 0 : 선 상
//	//// y - ax - b  < 0 : 선 아래쪽
//	//// y - ax - b  > 0 : 선 위쪽
//
//	//if (vPos.y - fGradient * vPos.x - fB[0] >= 0 &&
//	//	vPos.y + fGradient * vPos.x - fB[1] < 0 &&
//	//	vPos.y - fGradient * vPos.x - fB[2] < 0 &&
//	//	vPos.y + fGradient * vPos.x - fB[3] >= 0)
//	//{
//	//	return true;
//	//}
//
//	return false;
//}
//
//bool CBack::PickingDot(const D3DXVECTOR3& vPos, const int& iIndex)
//{
//
//	D3DXVECTOR3			vPoint[4] = {
//
//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + TILECY / 2.f, 0.f),
//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILECX / 2.f, m_vecTile[iIndex]->vPos.y, 0.f),
//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - TILECY / 2.f, 0.f),
//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILECX / 2.f, m_vecTile[iIndex]->vPos.y, 0.f)
//
//	};
//
//	D3DXVECTOR3			vDirection[4] = {
//
//		vPoint[1] - vPoint[0],
//		vPoint[2] - vPoint[1],
//		vPoint[3] - vPoint[2],
//		vPoint[0] - vPoint[3]
//
//	};
//
//
//	D3DXVECTOR3			vNormal[4] = {
//
//		D3DXVECTOR3(-vDirection[0].y, vDirection[0].x, 0.f),
//		D3DXVECTOR3(-vDirection[1].y, vDirection[1].x, 0.f),
//		D3DXVECTOR3(-vDirection[2].y, vDirection[2].x, 0.f),
//		D3DXVECTOR3(-vDirection[3].y, vDirection[3].x, 0.f)
//	};
//
//
//
//	for (size_t i = 0; i < 4; ++i)
//	{
//		D3DXVECTOR3	vDestDir = vPos - vPoint[i];
//		D3DXVec3Normalize(&vDestDir, &vDestDir);
//		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
//
//		float fDot = D3DXVec3Dot(&vDestDir, &vNormal[i]);
//
//		if (fDot > 0.f)
//			return false;
//	}
//
//	return true;
//}
//
//void CBack::LoadTile()
//{
//
//	HANDLE		hFile = CreateFile(L"../Data/Tile.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	DWORD		dwByte = 0;
//
//	while (true)
//	{
//		TILE*		pTile = new TILE;
//
//		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);
//
//		if (dwByte == 0)
//		{
//			Safe_Delete(pTile);
//			break;
//		}
//		m_vecTile.push_back(pTile);
//	}
//
//	CloseHandle(hFile);
//
//}
