#pragma once
#include "CEnemyBoss.h"

enum class eDoctorPattern
{
    ARM, CLONE, EYE, MONSTER
};

enum class eBossClonePattern
{
    ORBIT, LINE, RANDOM
};

enum class eBossEyePattern
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
    void UpdatePhaseArm(float fDeltaTime);
    void UpdatePhaseClone(float fDeltaTime);
    void UpdatePhaseEye(float fDeltaTime);
    void UpdatePhaseMonster(float fDeltaTime);
    //보스 팔 패턴
    void UpdateArmClone();
    //보스 클론 패턴
    void UpdateOrbitClone();
    void UpdateLineClone();
    void UpdateRandomClone();
    void Pattern_FallingBullet(float fDeltaTime);
    void SpawnFallingBullet();
    //눈 패턴
    void SpawnEyeClone(int iCount);
    bool IsEyeCloneDead();
    void Eye_Noraml(float fDeltaTime);
    void Eye_Divide(float fDeltaTime);
    void Eye_ReverseDivide(float fDeltaTime);
    void Eye_UpsideDown(float fDeltaTime);
    bool IsEyeSplitPhase();
    eBossEyePattern GetEyePattern() { return m_eEyePattern; }
private:
    eDoctorPattern m_eCurrentPattern;
    eBossClonePattern m_eClonePattern;
    eBossEyePattern m_eEyePattern;
    float m_fArmPatternTime;
    float m_fClonePatternTime;
public:
    static constexpr float kArmDuration = 5.f;
    static constexpr float kOrbitDuration = 3.f;
    static constexpr float kLineDuration = 3.f;
    static constexpr float kRandomDuration = 3.f;
};