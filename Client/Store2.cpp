#include "stdafx.h"
#include "Store2.h"

#include "ObjFactory.h"
#include "ObjMgr.h"

CStore2::CStore2()
	: m_fTime(5.f)
{
}


CStore2::~CStore2()
{
	Release();
}

HRESULT CStore2::Initialize(void)
{
	CBack* pBack = CObjFactory<CBack>::CreateObj();
	pBack->Set_Stage(L"Store2");
	pBack->Camera_Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_BACK, pBack);

	wstring wstrStage = pBack->Get_wstrStage(); 
	int iIndex = pBack->Get_Player_Index(wstrStage);
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(iIndex);

	
	// NPC
	m_pNpc = new CNpc(L"Quest");
	m_pNpc->Set_Pos(264);
	m_pNpc->Initialize();
	CObjMgr::Get_Instance()->AddObject(OBJ_NPC, m_pNpc);


	// ����Ʈ
	m_pMenuUI = static_cast<CMenuUI*>(CObjMgr::Get_Instance()->Get_MenuUI());
	m_pEventWnd = static_cast<CEventWnd*>(m_pMenuUI->Get_UI(CMenuUI::PT_EVENT));


	// Chat ����ִ� ���
	CChat* pChatUI = static_cast<CChat*>(CObjMgr::Get_Instance()->Get_ChatUI());
	m_ChatListPointer = pChatUI->Get_ChatListPtr();


	// ����
	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	if (!pPlayer->Is_Dead())
		CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Store.wav");
	else
		CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Boss.mp3");

	return S_OK;
}

int CStore2::Progress(void)
{
	m_fTime += GET_TIME;

	CChat* pChatUI = static_cast<CChat*>(CObjMgr::Get_Instance()->Get_ChatUI());
	m_bEnteredText = pChatUI->Was_Entered_Text();

	if (m_ChatListPointer == nullptr)
		return 0;

	// ����Ʈ ��
	D3DXVECTOR3 vPos = m_pNpc->Get_Info().vPos - CObj::m_vScroll;
	m_rcNPC = Calculate_Rect(vPos, 55.f, 100.f);
	
	POINT pt = GetMousePt();
	if (PtInRect(&m_rcNPC, pt))
	{
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON, CKeyMgr::S_NPC))
		{
			if (m_pEventWnd->m_iQuestProcess == 0)
			{
				m_bQuestShow = true;
				m_fTime = 0.f;

				CSoundMgr::Get_Instance()->StopAll();
				CSoundMgr::Get_Instance()->PlayBGM(L"bgm_quest.MP3");
			}

			else if (m_pEventWnd->m_iQuestProcess == 5)
			{
				CSoundMgr::Get_Instance()->StopAll();
				m_bQuestShow = true;
				m_fTime = 0.f;
			}
		}
	}

	// �����. no, yes �� �� �Է��� �� �ִ� ���°� �ȴ�
	if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
	{
		m_pEventWnd->m_iQuestProcess = 1;
	}

	if (!m_ChatListPointer->empty())
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());

		if (m_bEnteredText)
		{
			wstring wstrLast = m_ChatListPointer->back();

			if (m_pEventWnd->m_iQuestProcess == 1)
			{
				size_t n = wstrLast.find(L"no");
				if (n != std::wstring::npos)
				{
					m_fTime = 0.f;
					m_bQuestShow = true;
					m_pEventWnd->m_iQuestProcess = 2;
				}

				size_t n2 = wstrLast.find(L"yes");
				if (n2 != std::wstring::npos)
				{
					m_fTime = 0.f;
					m_bQuestShow = true;
					m_pEventWnd->m_iQuestProcess = 3;
				}
			}
			

			// �����
			if (pPlayer->Is_Dead())
			{
				size_t n1 = wstrLast.find(L"sos");
				size_t n2 = wstrLast.find(L"help");
				size_t n3 = wstrLast.find(L"Can you");
				size_t n4 = wstrLast.find(L"want");

				if (
					(n1 != std::wstring::npos) ||
					(n2 != std::wstring::npos) ||
					(n3 != std::wstring::npos) ||
					(n4 != std::wstring::npos)
					)
				{
					m_fTime = 0.f;
					m_bTextShow = true;
				}

				size_t n5 = wstrLast.find(L"please");
				if (n5 != std::wstring::npos && m_iRevivePoint == 0)
				{
					m_fTime = 0.f;
					m_iRevivePoint = 1;
					m_bTextShow = true;
				}
			}
			else
			{
				size_t n6 = wstrLast.find(L"thank");
				size_t n7 = wstrLast.find(L"Thank");
				if (
					((n6 != std::wstring::npos) || (n7 != std::wstring::npos))
					&& m_iRevivePoint == 1
					)
				{
					m_fTime = 0.f;
					m_iRevivePoint = 2;
					m_bTextShow = true;
				}
			}
		}
	}

	int iResult = 0;
	iResult = m_pObjMgr->Progress();
	return 0;
}

void CStore2::Render(void)
{
	if (m_ChatListPointer == nullptr)
		return;
	
	m_pObjMgr->Render();
	

	// ����Ʈ ��
	if (m_bQuestShow)
	{
		wstring wstrQuest = L"";

		if (m_pEventWnd->m_iQuestProcess == 0)
		{
			if (m_fTime <= 2.f)
			{
				wstrQuest = L"���� ������ �ʹ� �Ҷ�������";
			}
			else if (m_fTime <= 3.f)
			{
				wstrQuest = L"���Ͱ� �پ������ �ʾ�.....";
			}
			else if (m_fTime <= 3.5f)
			{
				wstrQuest = L"Ȥ�� �ڳ�.....";
			}
			else if (m_fTime <= 4.5f)
			{
				wstrQuest = L"�� ��Ź �� ����� �� �ְڴ°�";
			}
			else
			{
				m_bQuestShow = false;
				m_pEventWnd->m_iQuestProcess = 1;
			}
		}

		else if (m_pEventWnd->m_iQuestProcess == 2)
		{
			if (m_fTime <= 1.f)
			{
				wstrQuest = L"��... ��������?";
			}
			else if (m_fTime <= 2.f)
			{
				wstrQuest = L"�׷��ٸ� ��¿ �� ����";
			}
			else if (m_fTime <= 3.f)
			{
				wstrQuest = L"�ÿ�ȸ�� �����ַ� �߰Ŵ�..";
			}
			else if (m_fTime <= 5.f)
			{
				wstrQuest = L"���� ��,, ���� �����̵��̶�,,";
			}
			else if (m_fTime <= 5.5f)
			{
				wstrQuest = L"��...?";
			}
			else if (m_fTime <= 6.5f)
			{
				wstrQuest = L"�ƴ�, ����̶��?!";
			}
			else if (m_fTime <= 8.f)
			{
				wstrQuest = L"��������. ��մ� ģ���α�";
			}
			else if (m_fTime <= 9.f)
			{
				wstrQuest = L"�����ٸ� ���� �ۿ���";
			}
			else if (m_fTime <= 11.f)
			{
				wstrQuest = L"���� 10������ ��ƿ��ְ�";
			}
			else
			{
				CSoundMgr::Get_Instance()->StopAll();
				CSoundMgr::Get_Instance()->PlaySound(L"QuestOn.wav", CSoundMgr::EFFECT);
				m_bQuestShow = false;
				m_pEventWnd->m_iQuestProcess = 4;
				m_pEventWnd->Quest_Get();
				m_pEventWnd->Show(true);
				CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Store.wav");
			}
		}

		else if (m_pEventWnd->m_iQuestProcess == 3)
		{
			if (m_fTime <= 1.f)
			{
				wstrQuest = L"����!!!";
			}
			else if (m_fTime <= 2.f)
			{
				wstrQuest = L"�׷��ٸ� ���� �ۿ���";
			}
			else if (m_fTime <= 3.f)
			{
				wstrQuest = L"���� 10������ ��ƿ��ְ�";
			}
			else
			{
				m_bQuestShow = false;
				m_pEventWnd->m_iQuestProcess = 4;
				CSoundMgr::Get_Instance()->StopAll();
				CSoundMgr::Get_Instance()->PlaySound(L"QuestOn.wav", CSoundMgr::EFFECT);
				m_pEventWnd->Quest_Get();
				m_pEventWnd->Show(true);

				CSoundMgr::Get_Instance()->PlayBGM(L"BGM_Store.wav");
			}
		}

		else if (m_pEventWnd->m_iQuestProcess == 5)
		{
			if (m_fTime <= 1.f)
			{
				wstrQuest = L"������ 20������ ��ƿ��ٴ�!";
			}
			else if (m_fTime <= 2.f)
			{
				wstrQuest = L"���� ���ҳ�!!!";
			}
			else if (m_fTime <= 3.f)
			{
				wstrQuest = L"���� � ���ư� ������ �ϰԳ�";
			}
			else if (m_fTime <= 4.f)
			{
				wstrQuest = L"(������ �幵 ������)";
				CSoundMgr::Get_Instance()->StopAll();
				CSoundMgr::Get_Instance()->PlaySound(L"Success.wav", CSoundMgr::EFFECT);
				m_bQuestClear = true;
			}
			else
			{
				m_bQuestShow = false;
			}
		}

		m_vTextCenter = m_pNpc->Get_Info().vPos - CObj::m_vScroll - D3DXVECTOR3{ 0.f, 70.f, 0.f };
		m_rcText = Calculate_Rect(m_vTextCenter, 170.f, 30.f);

		const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(L"TextMessage");

		if (nullptr == pTexture)
			return;

		float fCX = float(pTexture->tImgInfo.Width) * 0.5f;
		float fCY = float(pTexture->tImgInfo.Height) * 0.5f;

		D3DXMATRIX	matTrans;

		D3DXMatrixTranslation(&matTrans, m_vTextCenter.x, m_vTextCenter.y, 0.f);
		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fCX, fCY, 0.f),
			NULL,
			D3DCOLOR_ARGB(200, 255, 255, 255));

		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
			CGraphicDev::Get_Instance()->GetSprite(),
			wstrQuest.c_str(),
			lstrlen(wstrQuest.c_str()),
			&m_rcText,
			DT_CENTER | DT_VCENTER,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	// �����
	if (m_bTextShow)
	{
		wstring wstrText = L"";

		if (m_iRevivePoint == 0)
		{
			if (m_fTime <= 1.f)
			{
				wstrText = L"ũ��...������ �����ϱ���";
			}
			else
			{
				m_bTextShow = false;
			}
		}

		else if (m_iRevivePoint == 1)
		{
			if (m_fTime <= 1.f)
			{
				wstrText = L"��ȣ��, ������������";
				CSoundMgr::Get_Instance()->StopAll();
			}
			else if (m_fTime <= 1.5f)
			{
				wstrText = L"�� ���� ������";
			}
			else if (m_fTime <= 2.f)
			{
				wstrText = L"�����ϰ� ����";
			}
			else if (m_fTime <= 3.f)
			{
				wstrText = L"����ֵ��� �ϰڴ�";
			}
			else if (m_fTime <= 4.f)
			{
				wstrText = L"�̹� ���� �� ������ �쵵��!";
			}
			else
			{
				CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
				pPlayer->Revive();
				m_bTextShow = false;
			}
		}

		else if (m_iRevivePoint == 2)
		{
			if (m_fTime <= 1.f)
			{
				wstrText = L"���ٴ�... ����;;";
			}
			else if (m_fTime <= 1.5f)
			{
				wstrText = L"��;; ���� �ƴϰ�";
			}
			else if (m_fTime <= 2.f)
			{
				wstrText = L"ġ�� �� ���ٰԢ�";
				CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
				pPlayer->Cure_From_Npc();
			}
			else
			{
				m_bTextShow = false;
			}
		}

		m_vTextCenter = m_pNpc->Get_Info().vPos - CObj::m_vScroll - D3DXVECTOR3{ 0.f, 70.f, 0.f };
		m_rcText = Calculate_Rect(m_vTextCenter, 170.f, 30.f);

		const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(L"TextMessage");

		if (nullptr == pTexture)
			return;

		float fCX = float(pTexture->tImgInfo.Width) * 0.5f;
		float fCY = float(pTexture->tImgInfo.Height) * 0.5f;

		D3DXMATRIX	matTrans;

		D3DXMatrixTranslation(&matTrans, m_vTextCenter.x, m_vTextCenter.y, 0.f);
		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fCX, fCY, 0.f),
			NULL,
			D3DCOLOR_ARGB(200, 255, 255, 255));

		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetFont_Normal12()->DrawTextW(
			CGraphicDev::Get_Instance()->GetSprite(),
			wstrText.c_str(),
			lstrlen(wstrText.c_str()),
			&m_rcText,
			DT_CENTER | DT_VCENTER,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	CGraphicDev::Get_Instance()->Draw_Rect(m_rcText);

	if (m_bQuestClear)
	{
		const TEXINFO* pTexture = CTextureMgr::Get_Instance()->GetTexture(L"Clear");

		if (nullptr == pTexture)
			return;

		float fCX = float(pTexture->tImgInfo.Width) * 0.5f;
		float fCY = float(pTexture->tImgInfo.Height) * 0.5f;

		D3DXMATRIX	matTrans;

		D3DXMatrixTranslation(&matTrans, WINCX*0.5f, WINCY*0.5f, 0.f);
		CGraphicDev::Get_Instance()->GetSprite()->SetTransform(&matTrans);
		CGraphicDev::Get_Instance()->GetSprite()->Draw(pTexture->pTexture,
			NULL,
			&D3DXVECTOR3(fCX, fCY, 0.f),
			NULL,
			D3DCOLOR_ARGB(200, 255, 255, 255));
	}
}

void CStore2::Release(void)
{
	CObjMgr::Get_Instance()->DeleteObject(OBJ_BACK);
	CObjMgr::Get_Instance()->DeleteObject(OBJ_NPC);
	CPlayer* pPlayer = static_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	CSoundMgr::Get_Instance()->StopAll();
}

void CStore2::Late_Progress(void)
{
	m_pObjMgr->Late_Progress();
}
