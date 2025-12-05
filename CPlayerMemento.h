#pragma once

struct CPlayerMemento
{
	//위치
	float fX;
	float fY;
	//속도
	float fVelX;
	float fVelY;
	//방향, 시선
	float fDirX;
	float fDirY;
	bool bFacingRight;
	//생사 
	bool bDead;
	//애니메이션 상태
	int iAnimState;
	//땅, 벽에 붙어있는지
	bool bOnGround;
	bool bAttackWall;
	//대쉬
	bool bUseDash;
	float fDashCoolTime;
	float fDashRemainTime;
	//공격
	bool bAttacking;
	float fAttackCoolTime;
	float fAttackRemainTime;
	//시간 조절
	float fSlowGauge;
};