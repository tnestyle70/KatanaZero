#pragma once
#include "CObj.h"

class CAttack : public CObj
{
public:
	CAttack();
	~CAttack();
public:
	void Initialize() override;
	int Update(float fDeltaTime) override;
	void Late_Update(float fDeltaTime) override;
	void Render(HDC hDC) override;
	void Release() override;
public:
	void OnAttack(CObj* pEnemy);
private:
	void FollowPlayer(float fDeltaTime);
private:
	CObj* m_pOwner;
	CObj* m_pEnemy;
	float m_fActiveTime;
	bool m_bSuccessAttack;
	bool m_bActive;
};