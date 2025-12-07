#pragma once
#include "CEnemy.h"
#include "Define.h"

enum class eCloneState
{
    APPEAR, AIM, SHOT, VANISH
};

enum class eClonePattern
{
    ORBIT_AND_STRIKE, LINE_STRIKE, RANDOM_STRIKE
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
    void OnHit(int iDamage)override;
    void UpdatePhase(float fDeltaTime);
    void UpdateOrbitAndAttack(float fDeltaTime);
    void UpdateOrbitMove(float fDeltaTime);
    void UpdateLineStrike(float fDeltaTime);
    void UpdateRandomStrike(float fDeltaTime);
    void SetPattern(eClonePattern ePattern) { m_ePattern = ePattern; }
    void SetOrbitOffset(float fOffset) { m_fOrbitOffset = fOffset; }
    void SetOrbitSpawnDuration(float fOrbitAppearDuration) { m_fOrbitSpawnAppearDuration = fOrbitAppearDuration; }
    void SetLineOffset(float fOffset) { m_fLineOffset = fOffset; }
    void SetLineSpawnAppearDuration(float fLineAppearDuration) { m_fLineSpawnAppearDuration = fLineAppearDuration; }
public:
    void CreateBullet();
    void CreateBulletNormal();
private:
    eCloneState m_eState;
    eClonePattern m_ePattern;
    //원형 스폰 패턴
    float m_fCenterX;
    float m_fCenterY;
    bool m_bStart;
    float m_fOrbitSpawnAppearDuration;
    float m_fOrbitVanishDuration;
    float m_fSpawnAppearDuration;
    float m_fOrbitOffset;
    float m_fOrbitDistance;
    float m_fOrbitSpeed;
    float m_fOrbitAngle;
    //랜덤 스폰 패턴
    float m_fRandomSpawnAppearDuration;
    float m_fAimDuration;
    float m_fRandomShotDuration;
    bool m_bShot;
    //일렬 패턴 
    float m_fLineOffset;
    float m_fLineSpawnAppearDuration;
    float m_fLineVanishDuration;
    float m_fOffset;
};