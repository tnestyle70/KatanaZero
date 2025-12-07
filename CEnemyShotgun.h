#pragma once
#include "CEnemy.h"
#include "IMemento.h"

struct EnemyShotgunSnapshot : ISnapshot
{
	float fX, fY;
	float fCX, fCY;
	float fAngle;
	float fSpeed;
	bool bDead;
};

class CEnemyShotgun : public CEnemy, public IMemento
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
	std::unique_ptr<ISnapshot> SaveSnapshot() const override;
	void LoadSnapshot(const ISnapshot& snapshot) override;
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