#pragma once
#include "Define.h"
#include "CObj.h"
#include "IMemento.h"
#include <deque>

struct PlayerSnapshot : ISnapshot
{
	//위치, 크기
	float fX, fY;
	float fCX, fCY;
	//속도, 방향
	float fVelX, fVelY;
	float fDirX, fDirY;
	//생사
	bool bDead;
	//애니메이션
	ePlayerAnim eAnimState;
	//땅, 벽
	bool bOnGround;
	bool bAttachWall;
	//대쉬
	bool bUseDash;
	float fDashCoolTime;
	float fDashRemainTime;
	//공격
	bool bAttacking;
	float fAttackCoolTime;
	float fAttackRemainTime;
};

class CInputManager;

class CPlayer : public CObj, public IMemento
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
	std::unique_ptr<ISnapshot> SaveSnapshot() const override;
	void LoadSnapshot(const ISnapshot& snap) override;
	PlayerSnapshot SaveSnapshotValue() const;
	void LoadSnapshotValue(const PlayerSnapshot& snap);
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
	deque<PlayerSnapshot> m_dHistory;
	bool m_bRewinding;
};
