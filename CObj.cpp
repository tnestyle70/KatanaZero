#include "pch.h"
#include "CObj.h"

#include "Vec2.h"
#include "CCamera.h"

CObj::CObj() :m_pTarget(nullptr), m_fSpeed(0.f), m_bDead(false), m_fAngle(0.f), m_fDistance(0.f), m_fDirX(0.f), m_fDirY(0.f)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tRect, sizeof(m_tRect));
}

CObj::~CObj()
{
}

void CObj::Update_Rect()
{
	/*
	Vec2 vScreen = CCamera::Get_Instance()->WorldToScreen({ m_tInfo.fX, m_tInfo.fY });

	m_tRect.left = (LONG)(vScreen.fX - m_tInfo.fCX * 0.5f);
	m_tRect.top = (LONG)(vScreen.fY - m_tInfo.fCY * 0.5f);
	m_tRect.right = m_tRect.left + (LONG)m_tInfo.fCX;
	m_tRect.bottom = m_tRect.top + (LONG)m_tInfo.fCY;
	*/
	
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCY * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCX * 0.5f));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
	
}
