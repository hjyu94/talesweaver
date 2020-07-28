#include "stdafx.h"
#include "SkillIcon.h"


CSkillIcon::CSkillIcon()
{
}

CSkillIcon::CSkillIcon(wstring wstrObjKey)
	: CItem()
{
	m_tItem.wstrObjKey = wstrObjKey;
	m_tItem.eItemID = IT_SKILL;
	m_fCoolTime = 5.f;
}


CSkillIcon::~CSkillIcon()
{
}
