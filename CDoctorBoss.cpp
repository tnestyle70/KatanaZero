#include "pch.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CDoctorBoss.h"
#include "CDoctorClone.h"
#include "CDoctorEyeClone.h"
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
	m_eCurrentPattern = eDoctorPattern::EYE;
	m_eEyePattern = eEyePattern::REVERSE_DIVIDE;
	m_fPatternTime = 3.f;
	m_fAngle = 0.f;
	m_fSpeed = 300.f;
	m_iHP = 100;
}

int CDoctorBoss::Update(float fDeltaTime)
{
	if (m_fPatternTime > 0.f)
	{
		m_fPatternTime -= fDeltaTime;
		if (m_fPatternTime <= 0.f)
		{
			UpdatePhase1(fDeltaTime);
			m_fPatternTime = 4.f;
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
	case eDoctorPattern::ARM_ATTACK:
		//Pattern_FallingBullet(fDeltaTime);
		UpdateOrbitClone();
		break;
	case eDoctorPattern::FALLING_BULLET:
		//Pattern_FallingBullet(fDeltaTime);
		UpdateLineClone();
		break;
	case eDoctorPattern::EYE:
		Pattern_EyeClone(fDeltaTime);
		//m_eCurrentPattern = eDoctorPattern::WAITING;
		break;
	case eDoctorPattern::WAITING:
		//눈 패턴 동안은 잠시 대기
		break;
	default:
		break;
	}
}

void CDoctorBoss::UpdateOrbitClone()
{
	float fOrbitOffset = 0.f;
	for (int i = 0; i < 5; ++i)
	{
		float fSpawnDuration = rand() % 10 + 2;
		CObj* pBossClone = CAbstractFactory<CDoctorClone>::Create(WINCX >> 1, 250.f);
		dynamic_cast<CDoctorClone*>(pBossClone)->SetPattern(eClonePattern::ORBIT_AND_STRIKE);
		dynamic_cast<CDoctorClone*>(pBossClone)->SetOrbitOffset(fOrbitOffset);
		dynamic_cast<CDoctorClone*>(pBossClone)->SetOrbitSpawnDuration(fSpawnDuration / 10);
		fOrbitOffset += 72.f;
		CObjMgr::Get_Instance()->Add_Object(OBJ_DOCTOR_CLONE, pBossClone);
	}
}

void CDoctorBoss::UpdateLineClone()
{
	float fLineOffset = 78.f;
	for (int i = 0; i < 10; ++i)
	{
		float fSpawnDuration = rand() % 14 + 2;
		CObj* pBossClone = CAbstractFactory<CDoctorClone>::Create(0.f + fLineOffset, 100.f);
		dynamic_cast<CDoctorClone*>(pBossClone)->SetPattern(eClonePattern::LINE_STRIKE);
		dynamic_cast<CDoctorClone*>(pBossClone)->SetLineOffset(fLineOffset);
		dynamic_cast<CDoctorClone*>(pBossClone)->SetLineSpawnAppearDuration(fSpawnDuration / 10);
		fLineOffset += 200.f;
		CObjMgr::Get_Instance()->Add_Object(OBJ_DOCTOR_CLONE, pBossClone);
	}
}

void CDoctorBoss::UpdateRandomSpawnClone()
{
	for (int i = 0; i < 5; ++i)
	{
		float fRandX = rand() % WINCX;
		float fRandY = rand() % WINCY;
		CObj* pBossClone = CAbstractFactory<CDoctorClone>::Create(fRandX, fRandY);
		CObjMgr::Get_Instance()->Add_Object(OBJ_DOCTOR_CLONE, pBossClone);
	}
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
	dynamic_cast<CBullet*>(pBullet)->SetOwner(this, eBulletOwner::ENEMY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pBullet);
}

void CDoctorBoss::Pattern_EyeClone(float fDeltaTime)
{
	switch (m_eEyePattern)
	{
	case eEyePattern::NORMAL:
		SpawnEyeClone(1);
		Eye_Noraml(fDeltaTime);
		break;
	case eEyePattern::DIVIDE:
		SpawnEyeClone(2);
		Eye_Divide(fDeltaTime);
		break;
	case eEyePattern::REVERSE_DIVIDE:
		SpawnEyeClone(3);
		Eye_ReverseDivide(fDeltaTime);
		break;
	case eEyePattern::UPSIDE_DOWN:
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
		m_eEyePattern = eEyePattern::DIVIDE;
	}
	return;
}

void CDoctorBoss::Eye_Divide(float fDeltaTime)
{
	if (IsEyeCloneDead())
	{
		m_eEyePattern = eEyePattern::REVERSE_DIVIDE;
	}
	return;
}

void CDoctorBoss::Eye_ReverseDivide(float fDeltaTime)
{
	if (IsEyeCloneDead())
	{
		m_eEyePattern = eEyePattern::UPSIDE_DOWN;
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
		&& (m_eEyePattern != eEyePattern::NORMAL);
}
