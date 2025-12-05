#include "pch.h"
#include "CTimeManager.h"

//싱글턴 패턴 전역적으로 하나만 존재하는 CTimeManager를 사용한다.
CTimeManager* CTimeManager::m_pInst = nullptr;

void CTimeManager::Initialize()
{
	m_dwPrevTime = GetTickCount64();
	m_fTimeScale = 1.f;
	m_fLerpDuration = 0.f;
}

void CTimeManager::Update()
{
	DWORD dwCurrentTime = GetTickCount64();
	DWORD dwDiff = dwCurrentTime - m_dwPrevTime;
	m_dwPrevTime = dwCurrentTime;
	//ms -> s
	m_fDeltaTime = dwDiff / 1000.f;
	
	//타임스케일 보간
	if (m_fLerpDuration > 0.f)
	{
		m_fDeltaTime *= m_fTimeScale;
		m_fLerpDuration -= m_fDeltaTime;
		if (m_fLerpDuration < 0.f)
		{
			m_fTimeScale = 1.f;
			m_fLerpDuration = 0.f;
		}
	}
}

void CTimeManager::SetSlowMotion(float fLerpTime)
{
	m_fTimeScale = fLerpTime;
	m_fLerpDuration = 0.05f;
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
