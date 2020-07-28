#pragma once
#include "Obj.h"
#include "MapMgr.h"
class CItem;
class CEtc;

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
	virtual void Late_Progress(void) override;

public:
	void				Load_File(void);
	int					Get_Tile_Index(D3DXVECTOR3 vPos, bool Is_Scroll_Needed = false);
	bool				PickingDot(D3DXVECTOR3 vPos, int iIndex);
	void				Enter_Gate(int iTileIdx, DIR eCurDir);
	void				Push_Item(CItem* pItem) { m_vecItem.push_back(pItem); }
	void				Delete_Item(CItem* pItem);
	void				Set_Stage(wstring wstrObjkey) 
						{
							m_tFrame.wstrObjKey = wstrObjkey; 
							Load_File();
						}
	void				Camera_Initialize(void);

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
	vector<TILE*>				m_vecTile;
	int							m_iTileX;
	int							m_iTileY;

	vector<CEtc*>				m_vecObj;
	
	vector<CItem*>				m_vecItem;

};

