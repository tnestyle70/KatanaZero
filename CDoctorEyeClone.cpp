#include "pch.h"
#include "CDoctorEyeClone.h"

#include "Vec2.h"
#include "CCamera.h"

CDoctorEyeClone::CDoctorEyeClone()
{
}

CDoctorEyeClone::~CDoctorEyeClone()
{
}

void CDoctorEyeClone::Initialize()
{
	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 100.f;
	m_eState = eEyeState::APPEAR;
	m_fAppearDuration = 10.f;
	m_fKillDuration = 10.f;
	m_fVanishDuration = 0.1f;
	m_iHP = 1;
}

int CDoctorEyeClone::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;

	if (m_iHP <= 0)
	{
		m_bDead = true;
		return DEAD;
	}

	UpdateState(fDeltaTime);

	__super::Update_Rect();

	return NOEVENT;
}

void CDoctorEyeClone::Late_Update(float fDeltaTime)
{
}

void CDoctorEyeClone::Render(HDC hDC)
{
	//월드 -> 화면(사분면 미러 포함)
	Vec2 vScreen = CCamera::Get_Instance()->WorldToScreen({ m_tInfo.fX, m_tInfo.fY });
	//화면 기준으로 임시 렉트 계산
	float fLeft = vScreen.fX - m_tInfo.fCX * 0.5f;
	float fTop = vScreen.fY - m_tInfo.fCY * 0.5f;
	float fRight = fLeft + m_tInfo.fCX;
	float fBottom = fTop + m_tInfo.fCY;

	Rectangle(hDC, (int)fLeft, (int)fTop, (int)fRight, (int)fBottom);
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CDoctorEyeClone::Release()
{
}

void CDoctorEyeClone::OnHit()
{
	m_bDead = true;
}

void CDoctorEyeClone::UpdateState(float fDeltaTime)
{
	switch (m_eState)
	{
	case eEyeState::APPEAR:
		if (m_fAppearDuration > 0.f)
		{
			m_fAppearDuration -= fDeltaTime;
			if (m_fAppearDuration < 0.f)
			{
				m_eState = eEyeState::KILL;
			}
			if (m_iHP <= 0)
			{
				m_eState = eEyeState::VANISH;
			}
		}
		break;
	case eEyeState::KILL:
		if (m_fKillDuration > 0.f)
		{
			m_fKillDuration -= fDeltaTime;
			if (m_fKillDuration < 0.f)
			{
				m_eState = eEyeState::VANISH;
			}
		}
		break;
	case eEyeState::VANISH:
		m_bDead = true;
		break;
	default:
		break;
	}
}