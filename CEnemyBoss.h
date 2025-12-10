#pragma once
#include "CEnemy.h"

enum class eBossState
{
	INTRO, PHASE1, STUNNED 
};

class CEnemyBoss : public CEnemy
{
public:
	CEnemyBoss() {};
	virtual ~CEnemyBoss() {};
public:
	virtual void Initialize()PURE;
	virtual int Update(float fDeltaTime)PURE;
	virtual void Late_Update(float fDeltaTime)PURE;
	virtual void Render(HDC hDC)PURE;
	virtual void Release()PURE;
public:
	virtual void OnHit()PURE;
private:
	eBossState m_eState;
	float m_fStateTime;
};