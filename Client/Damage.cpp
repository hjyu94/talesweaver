#include "stdafx.h"
#include "Damage.h"

#include "DataSubject.h"
#include "UIObserver.h"

CDamage::CDamage()
	: m_bCritical(false)
	, m_fScale(1.f)
	, m_fTime(0.f)
{
	m_eRenderID = RDR_EFFECT;
}

CDamage::CDamage(int iDamage)
	: m_bCritical(false)
	, m_fScale(1.f)
	, m_fTime(0.f)
{
	m_iDamage = iDamage;
	m_eRenderID = RDR_EFFECT;
}

CDamage::~CDamage()
{
	Release();
}

void CDamage::Update_World_Matrix(void)
{
}

void CDamage::Update_State(void)
{
}

HRESULT CDamage::Initialize(void)
{
	m_tFrame.wstrObjKey = L"Font";
	m_tFrame.wstrStateKey = L"PlayerDmgFont";

	/*m_pObserver = new CUIObserver;
	CDataSubject::Get_Instance()->Subscribe(m_pObserver);
*/
	m_iCount = 0;
	int iCopy = m_iDamage;
	while (iCopy != 0)
	{
		iCopy /= 10;
		++m_iCount;
	}
	return E_NOTIMPL;
}

int CDamage::Progress(void)
{
	switch (m_iProgress)
	{
	case 1:
		m_fScale += 0.05;
		if (m_fScale >= 1.5) m_iProgress++;
		break;

	case 2:
		m_fScale -= 0.05;
		if (m_fScale < 0.3) return OBJ_DEAD;
		break;
	}

	//m_fTime += GET_TIME;

	//if (m_fScale < 0)
	//{
	//	return OBJ_DEAD;
	//}	
	//else if (m_fScale <= 1.5)
	//{
	//	m_fScale += m_fTime * 2;
	//}
	//else if (m_fScale > 1.5)
	//{
	//	m_fScale -= m_fTime * 2;
	//}

	return 0;
}

void CDamage::Render(void)
{
	D3DXMATRIX	matTrans, matParent, matScale;
	//matParent = *static_cast<CUIObserver*>(m_pObserver)->Get_ObserverMatrix();
	D3DXMatrixScaling(&matScale, m_fScale, m_fScale, 0.f);

	int iCopy = m_iDamage;

	for (int i = 0; i < m_iCount; ++i)
	{
		int iNumerator = pow(10.f, m_iCount-i-1);
		int iSprite = iCopy / iNumerator;
		iCopy = iCopy % iNumerator;

		const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(
			m_tFrame.wstrObjKey, m_tFrame.wstrStateKey, iSprite
		);

		if (nullptr == pTexture)
			return;

		D3DXMatrixTranslation(&matTrans
			, m_tInfo.vPos.x - m_vScroll.x + 20.f * i
			, m_tInfo.vPos.y - m_vScroll.y - 50.f
			, 0.f);

		//m_tInfo.matWorld = matTrans * matParent;
		m_tInfo.matWorld = matScale* matTrans;

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if (m_bCritical)
	{
		const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(L"Critical");

		if (nullptr == pTexture)
			return;

		D3DXMatrixTranslation(&matTrans
			, m_tInfo.vPos.x - m_vScroll.x
			, m_tInfo.vPos.y - m_vScroll.y
			, 0.f);

		//m_tInfo.matWorld = matTrans * matParent;

		m_tInfo.matWorld = matScale* matTrans;

		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			NULL,
			NULL,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CDamage::Release(void)
{
	Safe_Delete(m_pObserver);
}
