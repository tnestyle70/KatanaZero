#pragma once
#pragma once
#include "Define.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();
public:
	virtual void Initialize()PURE;
	virtual int Update(float fDeltaTime)PURE;
	virtual void Late_Update(float fDeltaTime)PURE;
	virtual void Render(HDC hDC)PURE;
	virtual void Release()PURE;
public:
	virtual void Update_Rect();
public:
	void SetTarget(CObj* pTarget) { m_pTarget = pTarget; }
	CObj* GetTarget() { return m_pTarget; }
	void SetInfo(float fCX, float fCY, float fX, float fY) { m_tInfo.fCX = fCX, m_tInfo.fCY = fCY, m_tInfo.fX = fX, m_tInfo.fY = fY; }
	INFO* GetInfo() { return &m_tInfo; }
	float& GetAngle() { return m_fAngle; }
	const RECT* GetRect() const { return &m_tRect; }
	void SetAngle(float fAngle) { m_fAngle = fAngle; }
	void SetDead() { m_bDead = true; }
	bool IsDead() {return m_bDead;}
	void SetPos(float fX, float fY) { m_tInfo.fX = fX, m_tInfo.fY = fY; }
	void SetDir(float fDirX, float fDirY) { m_fDirX = fDirX, m_fDirY = fDirY; }
protected:
	INFO m_tInfo;
	RECT m_tRect;
	CObj* m_pTarget;
	float m_fSpeed;
	bool m_bDead;
	float m_fAngle;
	float m_fDistance;
	float m_fDirX;
	float m_fDirY;
};