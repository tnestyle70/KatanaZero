#include "pch.h"
#include "CTimeManager.h"

//싱글턴 패턴 전역적으로 하나만 존재하는 CTimeManager를 사용한다.
CTimeManager* CTimeManager::m_pInst = nullptr;

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

CTimeManager::~CTimeManager()
{
}
