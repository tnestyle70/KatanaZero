#pragma once
#include "Define.h"
#include "Vec2.h"

struct MirrorParam
{
	float fScreenX;
	float fScreenY;
};

class CScreenSplit
{
public:
	void Initialize(int iWinCX, int iWinCY);
	Vec2 ApplyMirror(const Vec2& vScreenPos, int iQuadrant) const;
	RECT& GetQuad(int iSide) { return m_quad[iSide]; }
private:
	RECT m_quad[4];
	MirrorParam m_mirror[4];
	float m_fCenterX = 0.f;
	float m_fCenterY = 0.f;
};
