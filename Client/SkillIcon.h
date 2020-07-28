#pragma once
#include "Item.h"
#include "Skill.h"
class CSkillIcon :
	public CItem
{
public:
	CSkillIcon();
	CSkillIcon(wstring wstrObjKey);
	virtual ~CSkillIcon();

public:
	float	Get_CoolTime(void) { return m_fCoolTime; }
	void	Update_CoolTime(void) { m_fCoolTime += GET_TIME; }
	void	Reset_CoolTime(void) { m_fCoolTime = 0.f; }
	

private:
	float	m_fCoolTime;
};

