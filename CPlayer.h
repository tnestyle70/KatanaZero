#pragma once
#include "Define.h"
#include "CObj.h"
#include "CPlayerMemento.h"

enum ePlayerAnim
{
	IDLE, MOVE ,DASH, GROUND_ATTACK, AIR_ATTCK, END_ANIM
};

class CInputManager;

class CPlayer : public CObj
{
public:
	CPlayer() {};
	virtual ~CPlayer();
public:
	void Initialize() override;
	int Update(float fDeltaTime) override;
	void Late_Update(float fDeltaTime) override;
	void Release() override;
	void Render(HDC hDC) override;
public:
	void UpdateAttack(float fDeltaTime);
	void UpdateDash(float fDeltaTime);
	void ResolveTileCollision();
	void GetKeyInput();
	void SetAttackDir();
	void TryParry();
public:
	//메멘토 복원 함수
	CPlayerMemento SaveToMemento() const;
	void RestoreFromMemento(const CPlayerMemento& memento);
private:
	CInputManager* m_pInput;
	//애니메이션
	ePlayerAnim m_eAnim;
	//중력/점프용 변수
	float m_fVelY;
	float m_fVelX;
	float m_fGravity;
	float m_fJumpPower;
	bool m_bOnGround;
	bool m_bAttachWall;
	//대쉬용 변수
	bool m_bUseDash;
	bool m_bIsInvincible;
	float m_fDashDir;
	float m_fDashCoolTime;
	float m_fDashDuration;
	float m_fInfiniteTime;
	//공격용 변수
	bool m_bAttacking;
	bool m_bAirAttack;
	//타이머
	float m_fAttackDuration;
	float m_fAttackCoolTime;
	float m_fAttackDirX;
	float m_fAttackDirY;
private:
	//메멘토 복원용 멤버 변수
	bool m_bRewindingLock;
};
