#include "pch.h"
#include "CTimeManager.h"

//싱글턴 패턴 전역적으로 하나만 존재하는 CTimeManager를 사용한다.
CTimeManager* CTimeManager::m_pInst = nullptr;

void CTimeManager::Initialize()
{
	m_dwPrevTime = GetTickCount64();
	m_fDeltaTime = 0.f;
	m_fTimeScale = 1.f;
	m_fTargetScale = 1.f;
	m_fStartScale = 1.f;
	m_fLerpDuration = 0.f;
	m_fLerpElapsed = 0.f;
	m_bSlowMotion = false;
}

void CTimeManager::Update()
{
	DWORD dwCurrentTime = GetTickCount64();
	DWORD dwDiff = dwCurrentTime - m_dwPrevTime;
	m_dwPrevTime = dwCurrentTime;
	//ms -> s
	m_fDeltaTime = dwDiff / 1000.f;
	/*
	//튀는 프레임 방지
	if (m_fDeltaTime > 0.033f)
		m_fDeltaTime = 0.033f;
		*/
	/*
	//타임스케일 보간
	if (m_fLerpDuration > 0.f)
	{
		m_fLerpElapsed += m_fDeltaTime;
		float t = std::min(m_fLerpElapsed / m_fLerpDuration, 1.f);
		//선형보간
		m_fTimeScale = m_fStartScale + (m_fTargetScale - m_fStartScale) * t;

		if (t >= 1.f)
		{
			m_fLerpDuration = 0.f;
			m_fLerpElapsed = 0.f;
			m_fTimeScale = m_fTargetScale;
		}
	}
	*/
}

void CTimeManager::SetSlowMotion(bool bEnable, float fLerpTime)
{
	if (m_bSlowMotion == bEnable)
		return;
	m_bSlowMotion = bEnable;
	m_fStartScale = m_fTimeScale;
	m_fTargetScale = bEnable ? 0.2f : 1.f;
	m_fLerpDuration = fLerpTime;
	m_fLerpElapsed = 0.f;
}

CTimeManager* CTimeManager::GetInst()
{
	if (m_pInst == nullptr)
	{
		m_pInst = new CTimeManager;
	}
	return m_pInst;
}

void CTimeManager::DestoryInst()
{
	if (m_pInst)
	{
		delete m_pInst;
		m_pInst = nullptr;
	}
}
