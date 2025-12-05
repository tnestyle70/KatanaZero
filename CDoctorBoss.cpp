#include "pch.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CDoctorBoss.h"
#include "CDoctorClone.h"
#include "CBullet.h"

CDoctorBoss::CDoctorBoss()
{
}

CDoctorBoss::~CDoctorBoss()
{
}

void CDoctorBoss::Initialize()
{
	m_tInfo = { WINCX >> 1, 200.f, 100.f, 100.f };
	m_eCurrentPattern = eDoctorPattern::FALLING_BULLET;
	m_fPatternTime = 3.f;
	m_fAngle = 0.f;
	m_fSpeed = 300.f;
}

int CDoctorBoss::Update(float fDeltaTime)
{
	if (m_fPatternTime > 0.f)
	{
		m_fPatternTime -= fDeltaTime;
		if (m_fPatternTime <= 0.f)
		{
			UpdatePhase1(fDeltaTime);
			m_fPatternTime = 3.f;
		}
	}

	m_tInfo.fX += m_fSpeed * fDeltaTime * cosf(m_fAngle * (PI / 180));
	m_tInfo.fY -= m_fSpeed * fDeltaTime * sinf(m_fAngle * (PI / 180));

	ResolveCollision();

	__super::Update_Rect();

	return NOEVENT;
}

void CDoctorBoss::Late_Update(float fDeltaTime)
{
}

void CDoctorBoss::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CDoctorBoss::Release()
{
}

void CDoctorBoss::ResolveCollision()
{
	float fRandAngle; 
	if (m_tRect.right >= WINCX)
	{
		m_tInfo.fX -= 1.f;
		fRandAngle = rand() % 360;
		m_fAngle = fRandAngle;
		//m_fAngle = -180.f;
	}
	if (m_tRect.left <= 0)
	{
		m_tInfo.fX += 1.f;
		fRandAngle = rand() % 360;
		m_fAngle = fRandAngle;
	}
	if (m_tRect.top <= 0)
	{
		m_tInfo.fY += 1.f;
		fRandAngle = rand() % 360;
		m_fAngle = fRandAngle;
	}
	if (m_tRect.bottom >= WINCY - 400.f)
	{
		m_tInfo.fY -= 1.f;
		fRandAngle = rand() % 360;
		m_fAngle = fRandAngle;
	}
}

void CDoctorBoss::UpdatePhase1(float fDeltaTime)
{
	switch (m_eCurrentPattern)
	{
	case eDoctorPattern::FALLING_BULLET:
		Pattern_FallingBullet(fDeltaTime);
		UpdateRandomSpawnClone();
		break;
	default:
		break;
	}
}

void CDoctorBoss::UpdateRandomSpawnClone()
{
	float fRandX = rand() % WINCX;
	float fRandY = rand() % WINCY;
	CObj* pBossClone = CAbstractFactory<CDoctorClone>::Create(fRandX, fRandY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_DOCTOR_CLONE, pBossClone);
}

void CDoctorBoss::Pattern_FallingBullet(float fDeltaTime)
{
	if (m_fPatternTime >= m_fNextSpawnTime)
	{
		SpawnFallingBullet();
		//다음 생성 시간 - 0~0.1초 사이의 랜덤 시간
		m_fNextSpawnTime += m_fSpawnInterval + (rand() % 100 / 1000.f);
	}
	if (m_fPatternTime >= 5.f)
	{
		m_fPatternTime = 0.f;
		m_eCurrentPattern = eDoctorPattern::FALLING_BULLET;
	}
}

void CDoctorBoss::SpawnFallingBullet()
{
	float fMargin = 80.f;
	float fX = fMargin + (rand() % int(WINCX - fMargin * 2));
	float fY = -50.f;

	CObj* pBullet = CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY);
	pBullet->SetTarget(CObjMgr::Get_Instance()->Get_Object(OBJ_PLAYER));
	dynamic_cast<CBullet*>(pBullet)->SetOwner(this);	
	CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pBullet);
}
