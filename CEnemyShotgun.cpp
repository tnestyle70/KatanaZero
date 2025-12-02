#include "pch.h"
#include "CEnemyShotgun.h"

CEnemyShotgun::CEnemyShotgun()
{
}

CEnemyShotgun::~CEnemyShotgun()
{
}

void CEnemyShotgun::Initialize()
{
	m_tInfo = { WINCX >> 1, WINCY >> 1, 100.f, 100.f };
}

int CEnemyShotgun::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;

	__super::Update_Rect();

	return NOEVENT;
}

void CEnemyShotgun::Late_Update(float fDeltaTime)
{
}

void CEnemyShotgun::Release()
{
}

void CEnemyShotgun::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}
