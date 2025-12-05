#include "pch.h"
#include "CParry.h"

CParry::CParry()
	: m_fLifeTime(0.1f), m_fAccTime(0.f), m_bActive(false), m_bFacingRight(false)
{
}

CParry::~CParry()
{
}

void CParry::Initialize()
{
	//패링 판정 넓이
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 60.f;
	m_bDead = false;
	m_bActive = true;
}

int CParry::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;

	//플레이어 추적
	FollowPlayer(fDeltaTime);

	//패링 경과 시간 체크
	m_fAccTime += fDeltaTime;
	if (m_fAccTime >= m_fLifeTime)
		m_bDead = true;

	__super::Update_Rect();

	return NOEVENT;
}

void CParry::Late_Update(float fDeltaTime)
{
}

void CParry::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CParry::Release()
{
}

void CParry::FollowPlayer(float fDeltaTime)
{
	const float m_fOffsetX = 30.f;
	if (m_bFacingRight)
	{
		m_tInfo.fX = m_pTarget->GetInfo()->fX + m_fOffsetX;
	}
	else
	{
		m_tInfo.fX = m_pTarget->GetInfo()->fX - m_fOffsetX;
	}
	m_tInfo.fY = m_pTarget->GetInfo()->fY;
}
