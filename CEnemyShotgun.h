#pragma once
#include "CEnemy.h"

class CEnemyShotgun : public CEnemy
{
public:
	CEnemyShotgun();
	~CEnemyShotgun();
public:
	void Initialize() override;
	int Update(float fDeltaTime) override;
	void Late_Update(float fDeltaTime) override;
	void Release() override;
	void Render(HDC hDC) override;
public:
	void OnHit(int iDamage)override;
public:
	void CreateBullet();
	void ResolveCollision();
	void SetBulletDir();
private:
	float m_fAttackCoolTime;
	float m_fStep;
	float m_fBulletAngle;
};