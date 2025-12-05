#include "pch.h"
#include "CParryBullet.h"

CParryBullet::CParryBullet()
{
}

CParryBullet::~CParryBullet()
{
}

void CParryBullet::Initialize()
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 10.f;
	m_fSpeed = 2000.f;
}

int CParryBullet::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;
	
	m_tInfo.fX += m_fDirX * m_fSpeed * fDeltaTime;
	m_tInfo.fY += m_fDirY * m_fSpeed * fDeltaTime;

	__super::Update_Rect();

	return NOEVENT;
}

void CParryBullet::Late_Update(float fDeltaTime)
{
}

void CParryBullet::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CParryBullet::Release()
{
}
