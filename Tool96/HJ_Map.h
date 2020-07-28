#pragma once

#include "Include.h"

class CTool96View;
class CMapTab;
class CUnitTab;
class CObj;

class CHJ_Map
{
public:
	CHJ_Map();
	~CHJ_Map();

/************************************************************************/
public:
	HRESULT				Initialize(void);
	int						Progress(void);
	void					Release(void);
							
	void					Render(void);
	void					Tile_Render(void);
	void					Stage_Render(void);
	void					Obj_Render(void);

public:
	HRESULT					Tile_Initialize();
	void					Create_Object(CPoint& point, OBJ_DATA tData);
	void					Reset_Object();
	void					Reset_Tile();
	void					Load_File();
	void					Tile_Change(const D3DXVECTOR3& vPos, const TILE_DRAW& iDrawID, const wstring& wstrInfo);
	void					Show_PrevObj(bool bIsShow) { m_bPrev = bIsShow; }

public: // Get
	CTool96View*			Get_pMainView(void) { return m_pMainView; }
	CMapTab*				Get_pMapTab(void) { return m_pMapTab; }
	CUnitTab*				Get_pUnitTab(void) { return m_pUnitTab; }

	// 타일
	vector<TILE*>&		Get_vecTile() { return m_vecTile; }
	void						Get_StageInfo(int& iTileX, int& iTileY);
	int							Get_TileIndex(const D3DXVECTOR3& vPos);

	// 오브젝트
	CObj*						Get_pPrevObj(void) { return m_pPrevObj; }
	vector<CObj*>*		Get_ObjVecPtr(void) { return &m_vecObject; }

public: // Set
	void						Set_MainView(CTool96View* pView) { m_pMainView = pView; }
	void						Set_Stage(CString _stage) { m_cstrStage = _stage; }
	void						Set_Scale(float _scale) { m_fScale = _scale; }
	void						Set_StageInfo(int iTileX, int iTileY)
								{
									m_iTileX = iTileX;
									m_iTileY = iTileY;
								}

private:
	bool						Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool						PickingDot(const D3DXVECTOR3& vPos, const int& iIndex);

/************************************************************************/

private:
	CTool96View*			m_pMainView = nullptr;
	CMapTab*				m_pMapTab = nullptr;
	CUnitTab*				m_pUnitTab = nullptr;
	CString					m_cstrStage;
	float						m_fScale;

	//# 타일
	vector<TILE*>			m_vecTile;
	int							m_iTileX;
	int							m_iTileY;
	int							m_iStageHeight;
	int							m_iStageWidth;

	//# 오브젝트
	CObj*						m_pPrevObj;
	bool						m_bPrev;
	vector<CObj*>			m_vecObject;

};

