#pragma once
#include "CObj.h"
#include "CEnemyBoss.h"

enum class eDoctorPattern
{
    FALLING_BULLET
};

class CDoctorBoss : public CEnemyBoss
{
public:
    CDoctorBoss();
    virtual ~CDoctorBoss();
public:
    virtual void Initialize() override;
    virtual int  Update(float fDeltaTime) override;
    virtual void Late_Update(float fDeltaTime) override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
private:
    void ResolveCollision();
    void UpdatePhase1(float fDeltaTime);
    void Pattern_FallingBullet(float fDeltaTime);
    void SpawnFallingBullet();
private:
    eDoctorPattern m_eCurrentPattern;
    float m_fPatternTime;
    float m_fNextSpawnTime;
    //패턴 파라미터
    float m_fSpawnInterval = 0.4f;
};