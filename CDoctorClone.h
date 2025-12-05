#pragma once
#include "CEnemy.h"
#include "Define.h"

enum class eCloneState
{
    APPEAR, AIM, SHOT, VANISH
};

class CDoctorClone : public CEnemy
{
public:
    CDoctorClone();
	virtual ~CDoctorClone();
public:
    void Initialize() override;
    int  Update(float fDeltaTime) override;
    void Late_Update(float fDeltaTime) override;
    void Render(HDC hDC) override;
    void Release() override;
public:
    void OnHit(int iDamage);
public:
    void CreateBullet();
private:
    eCloneState m_eState;
    float m_fAppearDuration;
    float m_fAimDuration;
    float m_fShotDuration;
    bool m_bShot;
};