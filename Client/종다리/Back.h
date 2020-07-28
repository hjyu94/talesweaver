#pragma once
#include "Obj.h"
class CItem;

class CBack :
	public CObj
{
public:
	CBack();
	//CBack(CObj* pObj);
	//CBack(wstring cstrStage, int iCount);
	virtual ~CBack();

public:
	// Inherited via CObj
	virtual HRESULT Initialize(void) override;
	virtual int Progress() override;
	virtual void Render(void) override;
	virtual void Release(void) override;

	virtual void Update_World_Matrix(void);
	virtual void Update_State(void);
	virtual void Late_Progress(void) override;

public:
	void				Load_Tile(void);
	int					Get_Tile_Index(D3DXVECTOR3 vPos, bool Is_Scroll_Needed = false);
	bool				PickingDot(D3DXVECTOR3 vPos, int iIndex);
	void				Enter_Gate(int iTileIdx, DIR eCurDir);
	void				Push_Item(CItem* pItem) { m_vecItem.push_back(pItem); }
	void				Delete_Item(CItem* pItem);

public:
	void				Get_Tile_Cnt(int& iTileX, int& iTileY);
	vector<TILE*>*		Get_TileVecPtr(void) { return &m_vecTile; }
	void				Get_Map_Size(int& iWidth, int& iHeight);
	void				Get_Option_Draw(D3DXVECTOR3 vPos, TILE_DRAW& eDraw, TILE_OPTION& eOption, bool Is_Scroll_Needed = false);
	int					Get_Player_Index(wstring wstrStage);
	wstring				Get_wstrStage() { return m_tFrame.wstrObjKey; }
public:
	//void	`Scroll_Lock();

private:
	int					m_iMap_Width;
	int					m_iMap_Height;

	vector<TILE*>		m_vecTile;
	int					m_iTileX;
	int					m_iTileY;
		
	vector<CItem*>				m_vecItem;
	map<wstring, BACK_INFO*>	m_mapBack;

};	

