#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXMATRIX matWorld;
}INFO;

typedef struct tagTile
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vSize;

	//BYTE			byOption; // MOVE, NO_MOVE
	//BYTE			byDrawID; //

	TILE_OPTION		eOption;
	TILE_DRAW		eDrawID;
	wstring			wstrInfo;
}TILE;

typedef struct tagObjData
{
	wstring		wstrObjKey;
	wstring		wstrStateKey;

	D3DXVECTOR3	vPos;
	int			iCount;
	int			iAlpha;
	float		fScale;

	// For Tool
	D3DXVECTOR3			m_vCenter;
	float				m_fRadius;
} OBJ_DATA;

typedef struct tagClientObjData
{
	wstring		wstrObjKey;
	wstring		wstrStateKey;
	int			iCount;
	D3DXMATRIX	matWorld;
} CLIENT_OBJ_DATA;

typedef struct tagFrame
{
	std::wstring	wstrObjKey;
	std::wstring	wstrStateKey;
	int				iSpriteCnt;
	int				iSpriteMax;
	float			fFrameSpeed;
	float			fFrameTime;
}FRAME;

typedef struct tagSpriteInfo
{
	int					iCount;
	std::wstring		wstrPath;
	float				fFrameSpeed; // ms
	bool				bReverse;
	D3DCOLOR			tColorKey;
	POINT				ptCenter;

	tagSpriteInfo(void)
		: wstrPath(L""), iCount(0)
		, fFrameSpeed(100), tColorKey(0), bReverse(false)
	{
		ZeroMemory(&ptCenter, sizeof(POINT));
	}

} SPRITE_INFO;


typedef	struct tagTexturePath
{
	std::wstring		wstrObjKey;
	std::wstring		wstrStateKey;
	SPRITE_INFO			tTool_Info;

	tagTexturePath(void)
		: wstrObjKey(L""), wstrStateKey(L"")
	{
		ZeroMemory(&tTool_Info, sizeof(tTool_Info));
	}

} IMGPATH;

typedef struct tagSingleTexPath
{
	wstring		wstrObjKey;
	wstring		wstrPath;
} SINGLE_IMGPATH;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;	// 객체(클래스) : 제공되는 기능 사용이 주 목적
	D3DXIMAGE_INFO			tImgInfo;   // 자료형을 저장하는 구조체 : 특정 수치 값을 저장하는 것이 주 목적
	SPRITE_INFO				tToolInfo;
}TEXINFO;

typedef struct tagAStarNode
{
	float			fCost;
	int				iIndex;
	tagAStarNode*	pPrev;
} NODE;

typedef struct tagPlayer
{
	unsigned int		iLevel;

	unsigned int		iStab;
	unsigned int		iHack;
	unsigned int		iInt;
	unsigned int		iDef;
	unsigned int		iMr;
	unsigned int		iDex;
	unsigned int		iAgi;

	unsigned int		iSp;
	unsigned int		iMaxSp;

	unsigned int		iExp;
	unsigned int		iLevelExp;
	unsigned int		iMaxExp;
	
	unsigned int		iSeed;
	int		iPoint;
	
	wstring				wstrName;
} PLAYER_INFO;

typedef struct tagStatus
{
	int		iHp;
	int		iMaxHp;
	int		iMp;
	int		iMaxMp;
} STATUS_INFO;

typedef struct tagBack
{
	unsigned int	iPlayerIndex;
	DIR				ePlayerDir;
} BACK_INFO;

typedef struct tagUnit
{
	wstring			m_wstrMap;
	wstring			m_wstrObjKey;
	int				m_iTileIndex;
	STATUS_INFO		m_tStatusData;
} UNIT_INFO;

typedef struct tagItem
{
	ITEMID			eItemID;
	wstring			wstrName;
	wstring			wstrObjKey;
	unsigned int	iCost;
	int				iCount;

	tagItem()
	{}

	tagItem(ITEMID eID, wstring Name, wstring ObjKey, int Cost)
	{
		eItemID = eID;
		wstrName = Name;
		wstrObjKey = ObjKey;
		iCost = Cost;
		iCount = 1;
	}

	tagItem(ITEMID eID, wstring Name, wstring ObjKey, int Cost, int Count)
	{
		eItemID = eID;
		wstrName = Name;
		wstrObjKey = ObjKey;
		iCost = Cost;
		iCount = Count;
	}

} ITEM_INFO;