//#pragma once
//
//#include "Include.h"
//
//class CTool96View;
//class CBack
//{
//public:
//	CBack();
//	~CBack();
//
//public:
//	const vector<TILE*>*		GetTile(void) { return &m_vecTile; }
//
//public:
//	HRESULT		Initialize(void);
//	int			Progress(void);
//	void		Render(void);
//	void		MiniRender(void);
//
//	void		Release(void);
//
//public:
//	void		TileChange(const D3DXVECTOR3& vPos, const int& iDrawID);
//	void		SetMainView(CTool96View* pView) { m_pMainView = pView; }
//	void		LoadTile();
//
//private:
//	int			GetTileIndex(const D3DXVECTOR3& vPos);
//	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
//	bool		PickingDot(const D3DXVECTOR3& vPos, const int& iIndex);
//
//
//private:
//	vector<TILE*>		m_vecTile;
//	CTool96View*		m_pMainView = nullptr;
//};
//
