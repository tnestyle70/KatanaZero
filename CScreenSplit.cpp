#include "pch.h"
#include "CScreenSplit.h"

void CScreenSplit::Initialize(int iWinCX, int iWinCY)
{
	m_fCenterX = iWinCX * 0.5f;
	m_fCenterY = iWinCY * 0.5f;

	m_mirror[0] = { +1, -1 };
	m_mirror[1] = { -1, -1 };
	m_mirror[2] = { +1, +1 };
	m_mirror[3] = { -1, +1 };

	m_quad[0] = { 0, 0, iWinCX / 2, iWinCY / 2 };
	m_quad[1] = { iWinCX / 2, 0, iWinCX, iWinCY / 2 };
	m_quad[2] = { 0, iWinCY / 2, iWinCX / 2, iWinCY };
	m_quad[3] = { iWinCX / 2, iWinCY / 2, iWinCX, iWinCY};
}

Vec2 CScreenSplit::ApplyMirror(const Vec2& vScreenPos, int iQuadrant) const
{
	const auto& mirrorParam = m_mirror[iQuadrant];

	Vec2 Out = vScreenPos;

	Out.fX = m_fCenterX + (Out.fX - m_fCenterX) * mirrorParam.fScreenX;
	Out.fY = m_fCenterY + (Out.fY - m_fCenterY) * mirrorParam.fScreenY;

	return Out;
}
