#include "pch.h"
#include "CBullet.h"

CBullet::CBullet()
{
}

CBullet::~CBullet()
{
}

void CBullet::Initialize()
{
	m_tInfo.fCX = 10.f;
	m_tInfo.fCY = 10.f;
	m_fSpeed = 300.f;
}

int CBullet::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;
	//방향 X 속도로 위치를 업데이트해준다.SetDir()으로 shotgunenemy의 위치를 기준으로 총알을 생성한다.
	m_tInfo.fX += m_fSpeed * m_fDirX;
	m_tInfo.fY += m_fSpeed * m_fDirY;

	return NOEVENT;
}

void CBullet::Late_Update(float fDeltaTime)
{
}

void CBullet::Release()
{
}

void CBullet::Render(HDC hDC)
{
}
