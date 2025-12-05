#pragma once
#include "CObj.h"

class CParry : public CObj
{
public:
	CParry();
	~CParry();
public:
	void Initialize() override;
	int Update(float fDeltaTime) override;
	void Late_Update(float fDeltaTime) override;
	void Render(HDC hDC) override;
	void Release() override;
public:
	void SetFacingRight(bool bFacingRight) { m_bFacingRight = bFacingRight; }
private:
	void FollowPlayer(float fDeltaTime);
private:
	float m_fLifeTime;
	float m_fAccTime;
	bool m_bActive;
	bool m_bFacingRight;
};