#include "stdafx.h"
#include "UIObserver.h"
#include "DataSubject.h"


CUIObserver::CUIObserver()
{
}


CUIObserver::~CUIObserver()
{
}

void CUIObserver::Update(int iFlag, void* pData)
{
	list<void*>*		pDataList = CDataSubject::Get_Instance()->GetDataList(iFlag);

	if (nullptr == pDataList)
		return;

	auto	iter = find(pDataList->begin(), pDataList->end(), pData);

	switch (iFlag)
	{
	case STATUS:
		m_tObserverStatus = *((STATUS_INFO*)(*iter));
		break;

	case PLAYER_MATRIX:
		m_matObserver = *((D3DXMATRIX*)(*iter));
		break;

	case PLAYER_MOVESTATE:
		m_ePlayerMoveState = *((CPlayer::MOVE_STATE*)(*iter));
		break;
		
	case PLAYER_STATUS:
		m_pPlayerStatus = ((PLAYER_INFO*)(*iter));
		break;

	case PLAYER_INVENTORY:
		m_vecPlayerInven = (vector<CItem*>*)(*iter);
		break;

	case PLAYER_DIR:
		m_ObserverDir = *((DIR*)(*iter));
		break;
	}
}
