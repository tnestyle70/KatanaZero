#pragma once
#include "CEnemyBoss.h"

enum class eDoctorPattern
{
    ARM_ATTACK, FALLING_BULLET, EYE, MONSTER, WAITING
};

enum class eEyePattern
{
    NORMAL, DIVIDE, REVERSE_DIVIDE, UPSIDE_DOWN
};

class CDoctorBoss : public CEnemyBoss
{
public:
    CDoctorBoss();
    virtual ~CDoctorBoss();
public:
    void Initialize() override;
    int  Update(float fDeltaTime) override;
    void Late_Update(float fDeltaTime) override;
    void Render(HDC hDC) override;
    void Release() override;
public:
    void OnHit()override;
public :
    void ResolveCollision();
    void UpdatePhase1(float fDeltaTime);
    //보스 클론 패턴
    void UpdateOrbitClone();
    void UpdateLineClone();
    void UpdateRandomSpawnClone();
    void Pattern_FallingBullet(float fDeltaTime);
    void SpawnFallingBullet();
    //눈 패턴
    void Pattern_EyeClone(float fDeltaTime);
    void SpawnEyeClone(int iCount);
    bool IsEyeCloneDead();
    void Eye_Noraml(float fDeltaTime);
    void Eye_Divide(float fDeltaTime);
    void Eye_ReverseDivide(float fDeltaTime);
    void Eye_UpsideDown(float fDeltaTime);
    bool IsEyeSplitPhase();
    eEyePattern GetEyePattern() { return m_eEyePattern; }
private:
    eDoctorPattern m_eCurrentPattern;
    eEyePattern m_eEyePattern;
    float m_fPatternTime;
    float m_fNextSpawnTime;
    //패턴 파라미터
    float m_fSpawnInterval = 0.4f;
};