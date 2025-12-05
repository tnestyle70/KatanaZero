#include "pch.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CDoctorClone.h"
#include "CBullet.h"

CDoctorClone::CDoctorClone()
{
}

CDoctorClone::~CDoctorClone()
{
}

void CDoctorClone::Initialize()
{
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 100.f;
	m_eState = eCloneState::APPEAR;
	m_fAppearDuration = 1.f;
	m_fAimDuration = 1.f;
	m_fShotDuration = 1.f;
	m_bShot = false;
}

int CDoctorClone::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;

	switch (m_eState)
	{
	case eCloneState::APPEAR:
		m_fAppearDuration -= fDeltaTime;
		if (m_fAppearDuration < 0.f)
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
		m_fShotDuration -= fDeltaTime;
		if(m_fShotDuration < 0.f)
			m_eState = eCloneState::VANISH;
		break;
	case eCloneState::VANISH:
		m_bDead = true;
		break;
	default:
		break;
	}

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
