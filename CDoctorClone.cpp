#include "pch.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CDoctorClone.h"
#include "CBullet.h"
#include "CBulletNormal.h"

CDoctorClone::CDoctorClone()
{
}

CDoctorClone::~CDoctorClone()
{
}

void CDoctorClone::Initialize()
{
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;
	m_eState = eCloneState::APPEAR;
	m_ePattern = eClonePattern::RANDOM_STRIKE;
	//원형 회전 패턴
	m_bStart = false;
	m_fDistance = 100.f;
	m_fOrbitSpawnAppearDuration = 1.f;
	m_fOrbitVanishDuration = 0.5f;
	m_fOrbitOffset = 0.f;
	m_fOrbitDistance = 100.f;
	m_fOrbitSpeed = 200.f;
	m_fOrbitAngle = 0.f;
	//랜덤 스폰 패턴
	m_fRandomSpawnAppearDuration = 0.2f;
	m_fAimDuration = 0.6f;
	m_fRandomShotDuration = 0.3f;
	m_bShot = false;
	//일렬 패턴
	m_fLineSpawnAppearDuration = 0.6f;
	m_fLineVanishDuration = 0.3f;
}

int CDoctorClone::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;

	UpdatePhase(fDeltaTime);

	__super::Update_Rect();

	return NOEVENT;
}

void CDoctorClone::Late_Update(float fDeltaTime)
{
	return;
}

void CDoctorClone::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CDoctorClone::UpdatePhase(float fDeltaTime)
{
	switch (m_ePattern)
	{
	case eClonePattern::ORBIT_AND_STRIKE:
		UpdateOrbitAndAttack(fDeltaTime);
		break;
	case eClonePattern::LINE_STRIKE:
		UpdateLineStrike(fDeltaTime);
		break;
	case eClonePattern::RANDOM_STRIKE:
		UpdateRandomStrike(fDeltaTime);
		break;
	default:
		break;
	}
}

void CDoctorClone::UpdateOrbitAndAttack(float fDeltaTime)
{
	switch (m_eState)
	{
	case eCloneState::APPEAR:
		UpdateOrbitMove(fDeltaTime);

		m_fOrbitSpawnAppearDuration -= fDeltaTime;

		if (m_fOrbitSpawnAppearDuration < 0.f)
			m_eState = eCloneState::SHOT;
		break;
	case eCloneState::AIM:
		break;
	case eCloneState::SHOT:
		if (!m_bShot)
		{
			CreateBullet();
			m_bShot = true;
		}
		m_fOrbitVanishDuration -= fDeltaTime;
		if (m_fOrbitVanishDuration < 0.f)
			m_eState = eCloneState::VANISH;
		break;
	case eCloneState::VANISH:
		m_bDead = true;
		break;
	default:
		break;
	}
}

void CDoctorClone::UpdateOrbitMove(float fDeltaTime)
{
	if (!m_bStart)
	{
		//초기 중심값 설정
		m_fCenterX = m_tInfo.fX;
		m_fCenterY = m_tInfo.fY;
		//생성할 때 오프셋만큼 초기 각도값에 더해줌
		m_fOrbitAngle += m_fOrbitOffset;
		m_bStart = true;
	}
	//원 궤적 따라서 이동
	m_tInfo.fX = m_fCenterX + m_fDistance * cosf(m_fOrbitAngle * (PI / 180));
	m_tInfo.fY = m_fCenterY - m_fDistance * sinf(m_fOrbitAngle * (PI / 180));

	m_fOrbitAngle += 300.f * fDeltaTime;
}

void CDoctorClone::UpdateLineStrike(float fDeltaTime)
{
	switch (m_eState)
	{
	case eCloneState::APPEAR:
		m_fLineSpawnAppearDuration -= fDeltaTime;
		if (m_fLineSpawnAppearDuration < 0.f)
			m_eState = eCloneState::SHOT;
		break;
	case eCloneState::AIM:
		break;
	case eCloneState::SHOT:
		if (!m_bShot)
		{
			CreateBulletNormal();
			m_bShot = true;
		}
		m_fLineVanishDuration -= fDeltaTime;
		if (m_fLineVanishDuration < 0.f)
			m_eState = eCloneState::VANISH;
		break;
	case eCloneState::VANISH:
		m_bDead = true;
		break;
	default:
		break;
	}
}

void CDoctorClone::UpdateRandomStrike(float fDeltaTime)
{
	switch (m_eState)
	{
	case eCloneState::APPEAR:
		m_fRandomSpawnAppearDuration -= fDeltaTime;
		if (m_fRandomSpawnAppearDuration < 0.f)
			m_eState = eCloneState::SHOT;
		break;
	case eCloneState::AIM:
		break;
	case eCloneState::SHOT:
		if (!m_bShot)
		{
			CreateBullet();
			m_bShot = true;
		}
		m_fRandomShotDuration -= fDeltaTime;
		if (m_fRandomShotDuration < 0.f)
			m_eState = eCloneState::VANISH;
		break;
	case eCloneState::VANISH:
		m_bDead = true;
		break;
	default:
		break;
	}
}

void CDoctorClone::Release()
{
}

void CDoctorClone::OnHit(int iDamage)
{
}

void CDoctorClone::CreateBullet()
{
	CObj* pBullet = CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pBullet);
	pBullet->SetTarget(CObjMgr::Get_Instance()->Get_Object(OBJ_PLAYER));
	dynamic_cast<CBullet*>(pBullet)->SetOwner(this);
}

void CDoctorClone::CreateBulletNormal()
{
	CObj* pBullet = CAbstractFactory<CBulletNormal>::Create(m_tInfo.fX, m_tInfo.fY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, pBullet);
	dynamic_cast<CBulletNormal*>(pBullet)->SetOwner(this);
}


