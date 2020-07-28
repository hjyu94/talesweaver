#pragma once

template <typename T>
static void Safe_Delete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
static void Safe_Delete_Array(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

template <typename T>
static DWORD Safe_Release(T ptr)
{
	DWORD dwRefCnt = 0;

	if (ptr)
		dwRefCnt = ptr->Release();

	return dwRefCnt;
}


struct DeleteObj
{
	template<typename T>
	void operator()(T& Temp)
	{
		if (Temp)
		{
			delete Temp;
			Temp = nullptr;
		}
	}
};


struct DeleteMap
{
	template<typename T>
	void operator()(T& Temp)
	{
		if (Temp.second)
		{
			delete Temp.second;
			Temp.second = nullptr;
		}
	}
};

class CMyStrCmp
{
public:
	CMyStrCmp() {}
	CMyStrCmp(const TCHAR* pString)
		:m_pString(pString)
	{}
public:
	template<class T>
	bool operator()(T& rPair)
	{
		return !lstrcmp(rPair.first, m_pString);
	}
private:
	const TCHAR* m_pString;
};

template<class T>
bool CompareY(T Dst, T Src)
{
	return Dst->Get_Info().vPos.y < Src->Get_Info().vPos.y;
}

template<class T>
bool ComparePriority(T Dst, T Src)
{
	return Dst->Get_Priority() < Src->Get_Priority();
}

static float Get_Distance(D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	D3DXVECTOR3 vDist = { abs(v1.x - v2.x), abs(v1.y - v2.y), 0.f };
	return sqrt(D3DXVec3Dot(&vDist, &vDist));
}

static RECT Calculate_Rect(D3DXVECTOR3 vPos, float fWidth, float fHeight, bool bIsPosCenter = true)
{
	RECT rc = {};
	if (bIsPosCenter)
	{
		rc =
		{
			(LONG)(vPos.x - fWidth*0.5f),
			(LONG)(vPos.y - fHeight*0.5f),
			(LONG)(vPos.x + fWidth*0.5f),
			(LONG)(vPos.y + fHeight*0.5f)
		};
	}
	else
	{
		rc =
		{
			(LONG)(vPos.x),
			(LONG)(vPos.y),
			(LONG)(vPos.x + fWidth),
			(LONG)(vPos.y + fHeight)
		};
	}

	return rc;
}

static D3DXVECTOR3		GetMouse(void)
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3(float(pt.x), float(pt.y), 0.f);
}

static POINT		GetMousePt(void)
{
	POINT	pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);
	return	pt;
}

//static void Scroll_Effect(RECT& tRect, D3DXVECTOR3 vScroll)
//{
//	tRect.left -= vScroll.x;
//	tRect.right -= vScroll.x;
//
//	tRect.top -= vScroll.y;
//	tRect.bottom -= vScroll.y;
//}