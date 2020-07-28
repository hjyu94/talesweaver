#pragma once

#include "Include.h"
class CTool96View;

class CObj
{
public:
	CObj();
	~CObj();

public:
	HRESULT		Initialize(void);
	int				Progress(void);
	//void			Render(void);
	void			Render(float fScale);
	void			Release(void);

public:
	const OBJ_DATA&		Get_Data(void) const { return m_tData; }
	OBJ_DATA*			Get_DataPtr(void) { return &m_tData; }
	D3DXVECTOR3&		Get_Center(void) { return m_tData.vPos; }

public:
	void	Set_Data(const OBJ_DATA& tData) {
		m_tData.fScale = tData.fScale;
		m_tData.iCount = tData.iCount;
		m_tData.vPos = tData.vPos;
		m_tData.wstrStateKey = tData.wstrStateKey.c_str();
		m_tData.iAlpha = tData.iAlpha;
	}

	void	Set_StateKey(const wstring& wstrStateKey) { m_tData.wstrStateKey = wstrStateKey; }
	void	Set_Pos(const CPoint& point) { m_tData.vPos = D3DXVECTOR3{ (float)point.x, (float)point.y, 0.f }; }
	void	Set_Alpha(const int& iAlpha) { m_tData.iAlpha = iAlpha; }
	void	Set_Scale(const float& fScale) { m_tData.fScale = fScale; }
	void	Set_Count(const int& iCount) { m_tData.iCount = iCount; }
	void	Set_Center(const CPoint& point) {
		m_tData.m_vCenter.x = (float)point.x;
		m_tData.m_vCenter.y = (float)point.y;
	}

private:
	OBJ_DATA		m_tData;
};

