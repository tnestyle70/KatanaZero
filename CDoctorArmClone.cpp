#include "pch.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CDoctorArmClone.h"

CDoctorArmClone::CDoctorArmClone()
{
}

CDoctorArmClone::~CDoctorArmClone()
{
}

void CDoctorArmClone::Initialize()
{
	m_iHP = 1;
	m_eArmState = eArmState::SPAWN;
	m_fSpawnTime = 1.f;
	m_fStrikeTime = 2.f;
	m_fHoldTime = 1.f;
	m_fRetractTime = 0.8f;
}

int CDoctorArmClone::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;

	if (m_iHP <= 0)
	{
		m_bDead = true;
		return DEAD;
	}

	UpdatePhase(fDeltaTime);

	__super::Update_Rect();

	return NOEVENT;
}

void CDoctorArmClone::Late_Update(float fDeltaTime)
{
}

void CDoctorArmClone::Render(HDC hDC)
{
}

void CDoctorArmClone::Release()
{
}

void CDoctorArmClone::OnHit()
{
	m_iHP -= 1;
}

void CDoctorArmClone::UpdatePhase(float fDeltaTime)
{
	
}
