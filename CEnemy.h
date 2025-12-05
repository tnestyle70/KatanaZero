#pragma once
#pragma once
#include "CObj.h"

enum class eEnemyState
{
	IDLE, MOVE, ATTACK, STUN, DIE 
};

class CEnemy : public CObj
{
public:
	CEnemy() {};
	virtual ~CEnemy() {};
public:
	virtual void Initialize()PURE;
	virtual int Update(float fDeltaTime)PURE;
	virtual void Late_Update(float fDeltaTime)PURE;
	virtual void Render(HDC hDC)PURE;
	virtual void Release()PURE;
public:
	virtual void OnHit(int iDamage)PURE;
protected:
	int m_iHP;
private:
	eEnemyState m_eState;
	float m_fStateTime;
};
