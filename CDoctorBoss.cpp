#include "pch.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CDoctorBoss.h"
#include "CDoctorClone.h"
#include "CDoctorEyeClone.h"
#include "CBullet.h"
#include <random>

CDoctorBoss::CDoctorBoss()
{
}

CDoctorBoss::~CDoctorBoss()
{
}

void CDoctorBoss::Initialize()
{
	m_tInfo = { WINCX >> 1, 200.f, 100.f, 100.f };
	m_eCurrentPattern = eDoctorPattern::CLONE;
	m_eClonePattern = eBossClonePattern::ORBIT;
	m_eEyePattern = eBossEyePattern::NORMAL;
	m_fArmPatternTime = 5.f; //팔 패턴
	m_fClonePatternTime = 3.f; //클론 패턴
	m_fAngle = 0.f;
	m_fSpeed = 300.f;
	m_iHP = 100;
	m_iArmCount = 4;
	m_iCloneCount = 10;
}

int CDoctorBoss::Update(float fDeltaTime)
{
	switch (m_eCurrentPattern)
	{
	case eDoctorPattern::ARM:
		UpdatePhaseArm(fDeltaTime);
		break;
	case eDoctorPattern::CLONE:
		UpdatePhaseClone(fDeltaTime);
		break;
	case eDoctorPattern::EYE:
		UpdatePhaseEye(fDeltaTime);
		break;
	case eDoctorPattern::MONSTER:
		UpdatePhaseMonster(fDeltaTime);
		break;
	default:
		break;
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

void CDoctorBoss::OnHit()
{
	//Hit시 처리
	m_iHP -= 1;
}

void CDoctorBoss::ResolveCollision()
{
	float fRandAngle;

	if (m_tRect.right >= WINCX)
	{
		m_tInfo.fX -= 1.f;
		fRandAngle = rand() % 360;
		m_fAngle = fRandAngle;
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

void CDoctorBoss::UpdatePhaseArm(float fDeltaTime)
{
	//조건 분기 달성 후 UpdatePhaseClone으로 넘어감
	if (m_fArmPatternTime > 0.f)
	{
		m_fArmPatternTime -= fDeltaTime;
		if (m_fArmPatternTime < 0.f)
		{
			UpdateArmClone();
			m_fArmPatternTime = kArmDuration;
		}
	}
}

void CDoctorBoss::UpdateArmClone()
{

}

void CDoctorBoss::UpdatePhaseClone(float fDeltaTime)
{
	switch (m_eClonePattern)
	{
	case eBossClonePattern::ORBIT:
		if (m_fClonePatternTime > 0.f)
		{
			m_fClonePatternTime -= fDeltaTime;
			if (m_fClonePatternTime < 0.f)
			{
				UpdateOrbitClone();
				m_eClonePattern = eBossClonePattern::LINE;
				m_fClonePatternTime = kLineDuration;
			}
		}
		break;
	case eBossClonePattern::LINE:
		if (m_fClonePatternTime > 0.f)
		{
			m_fClonePatternTime -= fDeltaTime;
			if (m_fClonePatternTime < 0.f)
			{
				UpdateLineClone();
				m_eClonePattern = eBossClonePattern::RANDOM;
				m_fClonePatternTime = kRandomDuration;
			}
		}
		break;
	case eBossClonePattern::RANDOM:
		if (m_fClonePatternTime > 0.f)
		{
			m_fClonePatternTime -= fDeltaTime;
			if (m_fClonePatternTime < 0.f)
			{
				UpdateRandomClone();
				m_eClonePattern = eBossClonePattern::ORBIT;
				m_fClonePatternTime = kOrbitDuration;
			}
		}
		break;
	default:
		break;
	}
}

void CDoctorBoss::UpdateOrbitClone()
{
	const int N = m_iCloneCount;
	if (N <= 0) return;
	const float stepDeg = 360.f / (float)N;

	float fOrbitOffset = 0.f;
	for (int i = 0; i < N; ++i)
	{
		float fSpawnDuration = (rand() % 10 + 2) / 5.f;
		CObj* pBossClone = CAbstractFactory<CDoctorClone>::Create(WINCX >> 1, 250.f);
		dynamic_cast<CDoctorClone*>(pBossClone)->SetPattern(eClonePattern::ORBIT_AND_STRIKE);

		const float fOrbitOffsetDeg = stepDeg * i;
		dynamic_cast<CDoctorClone*>(pBossClone)->SetOrbitOffset(fOrbitOffsetDeg);
		dynamic_cast<CDoctorClone*>(pBossClone)->SetOrbitSpawnDuration(fSpawnDuration);
		CObjMgr::Get_Instance()->Add_Object(OBJ_DOCTOR_CLONE, pBossClone);
	}
}

void CDoctorBoss::UpdateLineClone()
{
	const float fLeftMargin = 78.f;
	const float fRightMargin = 78.f;
	const int N = m_iCloneCount;
	if (N <= 0) return;
	const float fSpan = (WINCX - fLeftMargin - fRightMargin);
	const float fSpacing = (N > 1) ? (fSpan / (N - 1)) : 0.f;

	for (int i = 0; i < N; ++i)
	{
		float fSpawnX = fLeftMargin + fSpacing * i;

		float fSpawnDuration = (rand() % 14 + 2) / 5.f; 
		CObj* pBossClone = CAbstractFactory<CDoctorClone>::Create(fSpawnX, 100.f);
		dynamic_cast<CDoctorClone*>(pBossClone)->SetPattern(eClonePattern::LINE_STRIKE);
		dynamic_cast<CDoctorClone*>(pBossClone)->SetLineOffset(fSpawnX);
		dynamic_cast<CDoctorClone*>(pBossClone)->SetLineSpawnAppearDuration(fSpawnDuration);
		CObjMgr::Get_Instance()->Add_Object(OBJ_DOCTOR_CLONE, pBossClone);
	}
}

void CDoctorBoss::UpdateRandomClone()
{
	static std::mt19937 rng{ std::random_device{}() };
	constexpr float kMargin = 32.f;
	std::uniform_real_distribution<float> distX(kMargin, WINCX - kMargin);
	std::uniform_real_distribution<float> distY(kMargin, WINCY - kMargin);

	for (int i = 0; i < m_iCloneCount; ++i)
	{
		float fX = distX(rng);
		float fY = distY(rng);
		CObj* pBossClone = CAbstractFactory<CDoctorClone>::Create(fX, fY);
		CObjMgr::Get_Instance()->Add_Object(OBJ_DOCTOR_CLONE, pBossClone);
	}
}

void CDoctorBoss::UpdatePhaseEye(float fDeltaTime)
{
	switch (m_eEyePattern)
	{
	case eBossEyePattern::NORMAL:
		SpawnEyeClone(1);
		Eye_Noraml(fDeltaTime);
		break;
	case eBossEyePattern::DIVIDE:
		SpawnEyeClone(2);
		Eye_Divide(fDeltaTime);
		break;
	case eBossEyePattern::REVERSE_DIVIDE:
		SpawnEyeClone(3);
		Eye_ReverseDivide(fDeltaTime);
		break;
	case eBossEyePattern::UPSIDE_DOWN:
		SpawnEyeClone(5);
		Eye_UpsideDown(fDeltaTime);
		break;
	default:
		break;
	}
}

void CDoctorBoss::SpawnEyeClone(int iCount)
{
	for (int i = 0; i < iCount; ++i)
	{
		float fRandX = rand() % WINCX;
		float fRandY = rand() % WINCY;
		CObj* pEyeClone = CAbstractFactory<CDoctorEyeClone>::Create(fRandX, fRandY);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EYE_CLONE, pEyeClone);
	}
}

void CDoctorBoss::Eye_Noraml(float fDeltaTime)
{
	if (IsEyeCloneDead())
	{
		m_eEyePattern = eBossEyePattern::DIVIDE;
	}
	return;
}

void CDoctorBoss::Eye_Divide(float fDeltaTime)
{
	if (IsEyeCloneDead())
	{
		m_eEyePattern = eBossEyePattern::REVERSE_DIVIDE;
	}
	return;
}

void CDoctorBoss::Eye_ReverseDivide(float fDeltaTime)
{
	if (IsEyeCloneDead())
	{
		m_eEyePattern = eBossEyePattern::UPSIDE_DOWN;
	}
	return;
}

void CDoctorBoss::Eye_UpsideDown(float fDeltaTime)
{

}

bool CDoctorBoss::IsEyeCloneDead()
{
	auto listEye = CObjMgr::Get_Instance()->Get_Object_List(OBJ_EYE_CLONE);
	return listEye->empty();
}


bool CDoctorBoss::IsEyeSplitPhase() 
{
	return (m_eCurrentPattern == eDoctorPattern::EYE)
		&& (m_eEyePattern != eBossEyePattern::NORMAL);
}

void CDoctorBoss::UpdatePhaseMonster(float fDeltaTime)
{

}
/*
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
	dynamic_cast<CBullet*>(pBullet)->SetOwner(this, eBulletOwner::ENEMY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pBullet);
}
*/