#include "pch.h"
#include "Define.h"
#include "CEnemyShotgun.h"
#include "CAbstractFactory.h"

CEnemyShotgun::CEnemyShotgun()
{
}

CEnemyShotgun::~CEnemyShotgun()
{
}

void CEnemyShotgun::Initialize()
{
	m_tInfo = { WINCX >> 1, WINCY - 50.f, 100.f, 100.f };
	//처음 바라보는 방향 180도, 오른쪽으로 이동하면 0도
	m_fAngle = 45.f;
	m_fSpeed = 300.f;
}

int CEnemyShotgun::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;

	SetShotGunEnemyDir();

	m_tInfo.fX += m_fSpeed * m_fDirX * fDeltaTime;
	m_tInfo.fY += m_fSpeed * m_fDirY * fDeltaTime;

	ResolveCollision();

	__super::Update_Rect();

	return NOEVENT;
}

void CEnemyShotgun::Late_Update(float fDeltaTime)
{

}

void CEnemyShotgun::Release()
{
	
}

void CEnemyShotgun::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CEnemyShotgun::SetShotGunEnemyDir()
{
	//방향 값 계산
	m_fDirX = cosf(m_fAngle / (180 / PI));
	m_fDirY = -sinf(m_fAngle / (180 / PI));
}

void CEnemyShotgun::ResolveCollision()
{
	//충돌 처리 
	if (m_tRect.left <= 50.f)
	{
		//m_tInfo.fX += m_tInfo.fCX * 0.5f;
		m_fAngle = 180.f - m_fAngle;
	}
	if (m_tRect.right >= WINCX - 50.f)
	{
		//m_tInfo.fX += WINCX - m_tInfo.fCX * 0.5f;
		m_fAngle = 180.f - m_fAngle;
	}
	if (m_tRect.bottom >= WINCY - 50.f)
	{
		//m_tInfo.fX += m_tInfo.fCX * 0.5f;
		m_fAngle = 360.f - m_fAngle;
	}
	if (m_tRect.top <= 50.f)
	{
		//m_tInfo.fX += WINCX - m_tInfo.fCX * 0.5f;
		m_fAngle = 360.f - m_fAngle;
	}
}

void CEnemyShotgun::SetBulletDir()
{
	//적이 바라보는 방향 기준으로 5개의 총알이 발사되도록 구현
}
