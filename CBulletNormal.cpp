#include "pch.h"
#include "CBulletNormal.h"

CBulletNormal::CBulletNormal()
{
}

CBulletNormal::~CBulletNormal()
{
}

void CBulletNormal::Initialize()
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 10.f;
	m_fSpeed = 1500.f;
	m_bStart = false;
	m_fAngle = 90.f;
}

int CBulletNormal::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;

	m_tInfo.fX += m_fSpeed * fDeltaTime * cosf(m_fAngle * (PI / 180));
	m_tInfo.fY += m_fSpeed * fDeltaTime * sinf(m_fAngle * (PI / 180));

	__super::Update_Rect();

	return NOEVENT;
}

void CBulletNormal::Late_Update(float fDeltaTime)
{
}

void CBulletNormal::Release()
{
}

void CBulletNormal::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CBulletNormal::OnParried()
{
}
