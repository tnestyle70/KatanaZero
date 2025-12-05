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
	void OnParry(CObj* pBullet) { m_pBullet = pBullet; }
	void SuccessParry() { m_bSuccessParry = true; }
	void SetOwner(CObj* pOwner) { pOwner = m_pOwner; }
	void CreateParryBullet();
private:
	void FollowPlayer(float fDeltaTime);
private:
	CObj* m_pOwner;
	CObj* m_pBullet;
	float m_fActiveTime;
	bool m_bSuccessParry;
	bool m_bActive;
};