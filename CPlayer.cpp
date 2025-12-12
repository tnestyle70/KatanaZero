#include "pch.h"
#include "CPlayer.h"
#include  "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CParry.h"
#include "CAttack.h"
#include "CInputManager.h"

#include "Vec2.h"
#include "CCamera.h"

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	m_tInfo = { WINCX >> 1, WINCY >> 1, 100.f, 100.f };
	m_fSpeed = 300.f;
	//중력 점프용 변수
	m_fVelX = 0.f;
	m_fVelY = 0.f;
	m_fGravity = 2500.f;
	m_fJumpPower = 1000.f;
	m_bOnGround = false;
	m_bAttachWall = false;
	//대쉬용 변수
	m_bUseDash = false;
	m_bIsInvincible = false;
	m_fDashDir = 0.f;
	m_fDashCoolTime = 0.f;
	m_fDashDuration = DASH_DURATION;
	m_fInfiniteTime = DASH_INFINITE_FRAME_TIME;
	//공격용 변수 
	m_bAttacking = false;
	m_bAirAttack = false;
	m_fAttackCoolTime = 0.f;
	m_fAttackDuration = ATTACK_DURATION;
	m_fAttackDirX = 0.f;
	m_fAttackDirY = 0.f;
	//InputManager 연결
	m_pInput = CInputManager::GetInst();
	//메멘토
	m_bRewinding = false;
}

int CPlayer::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;
	/*
	if (m_pInput->KeyPress(eKey::REWIND))
	{
		m_bRewinding = true;

		if (!m_dHistory.empty())
		{
			LoadSnapshotValue(m_dHistory.back());
			m_dHistory.pop_back();
		}
		//되감기 중엔 정상 물리/입력 스킵
		return NOEVENT;
	}
	else
	{
		m_bRewinding = false;
	}
	
	//2.정상 상태일 때 Memento 저장
	m_dHistory.push_back(SaveSnapshotValue());
	//메모리 폭주 방지
	const size_t MAX_HISTORY = 30000;
	if (m_dHistory.size() > MAX_HISTORY)
	{
		m_dHistory.pop_front();
	}
	*/
	GetKeyInput();

	//대쉬 쿨타임 감소
	if (m_fDashCoolTime > 0.f)
	{
		m_fDashCoolTime -= fDeltaTime;
		if (m_fDashCoolTime < 0.f)
		{
			m_fDashCoolTime = 0.f;
		}
	}
	//공격 쿨타임 감소
	if (m_fAttackCoolTime > 0.f)
	{
		m_fAttackCoolTime -= fDeltaTime;
		if (m_fAttackCoolTime < 0.f)
		{
			m_fAttackCoolTime = 0.f;
		}
	}
	UpdateAttack(fDeltaTime);
	//중력 적용
	//m_fVelY += m_fGravity * fDeltaTime;
	UpdateDash(fDeltaTime);
	
	UpdateAttachWall(fDeltaTime);

	//중력 적용
	m_fVelY += m_fGravity * fDeltaTime;
	

	//중력, Dash, Attack, 물리가 적용된 결과 위치 적용
	m_tInfo.fX += m_fVelX * fDeltaTime;
	m_tInfo.fY += m_fVelY * fDeltaTime;

	__super::Update_Rect();

	ResolveTileCollision();

	__super::Update_Rect();

	return NOEVENT;
}

void CPlayer::UpdateAttack(float fDeltaTime)
{
	if (m_bAttacking && m_fAttackCoolTime <= 0.f)
	{
		m_fAttackDuration -= fDeltaTime;
		m_fVelX = m_fAttackDirX * ATTACK_LUNGE_SPEED;
		m_fVelY = m_fAttackDirY * ATTACK_LUNGE_SPEED;
		if (m_fAttackDuration < 0.f)
		{
			m_bAttacking = false;
			m_fAttackDuration = ATTACK_DURATION;
			m_fAttackCoolTime = ATTACK_COOLTIME;
		}
	}
}

void CPlayer::UpdateDash(float fDeltaTime)
{
	if (m_bUseDash && m_fDashCoolTime <= 0.f)
	{
		//Dash의 지속 시간 동안 m_VelX를 DASH_SPEED로 변경
		m_fDashDuration -= fDeltaTime;
		m_fVelX = DASH_SPEED * m_fDashDir;
		if (m_fDashDuration < 0.f)
		{
			m_bUseDash = false;
			m_bIsInvincible = false;
			m_fDashCoolTime = DASH_COOMTIME;
			m_fDashDuration = DASH_DURATION;
		}
	}
}

void CPlayer::UpdateAttachWall(float fDeltaTime)
{
	//벽에 닿았을 경우
	if (m_bAttachWall)
	{
		m_fVelY += (m_fGravity * 0.5f) * fDeltaTime;

		// 벽에 붙어 있을 땐 낙하 속도 상한 제한
		if (m_fVelY > 120.f)
			m_fVelY = 120.f;
	}
}

void CPlayer::ResolveTileCollision()
{
	const float fHalfW = m_tInfo.fCX * 0.5f;
	const float fHalfH = m_tInfo.fCY * 0.5f;

	if (m_tInfo.fY + fHalfH >= WINCY)
	{
		m_tInfo.fY = WINCY - fHalfH;
		m_fVelY = 0.f; //땅에 닿았을 때 프레임마다 중력 0 초기화 
		m_bOnGround = true;
	}
	//충돌 처리 
	if (m_tInfo.fX - fHalfW <= 0)
	{
		m_tInfo.fX = fHalfW;
		m_bAttachWall = true;
	}
	else if(m_tInfo.fX - fHalfW > 0)
		m_bAttachWall = false;

	if (m_tInfo.fX + fHalfW >= WORLDX)
	{
		m_tInfo.fX = WORLDX - fHalfW;
		m_bAttachWall = true;
	}
	//각도 보정
	if (m_fAngle <= 0.f) m_fAngle += 360.f;
	else if (m_fAngle >= 360.f) m_fAngle -= 360.f;
}

void CPlayer::Late_Update(float fDeltaTime)
{
}

void CPlayer::Release()
{
}

void CPlayer::Render(HDC hDC)
{
	//월드 -> 화면(사분면 미러 포함)
	Vec2 vScreen = CCamera::Get_Instance()->WorldToScreen({m_tInfo.fX, m_tInfo.fY});
	//화면 기준으로 임시 렉트 계산
	float fLeft = vScreen.fX - m_tInfo.fCX * 0.5f;
	float fTop = vScreen.fY - m_tInfo.fCY * 0.5f;
	float fRight = fLeft + m_tInfo.fCX;
	float fBottom = fTop + m_tInfo.fCY;
	
	Rectangle(hDC, (int)fLeft, (int)fTop, (int)fRight, (int)fBottom);
	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CPlayer::GetKeyInput()
{
	//프레임마다 VelX 초기화
	m_fVelX = 0.f;
	//WASD 이동
	if (m_bOnGround && m_pInput->KeyPress(eKey::UP))
	{
		m_fVelY = -m_fJumpPower;
		m_bOnGround = false;
	}
	if (m_pInput->KeyPress(eKey::DOWN)) {} //웅크리기 
	if (m_pInput->KeyPress(eKey::LEFT)) 
		m_fVelX = -m_fSpeed;
	if (m_pInput->KeyPress(eKey::RIGHT)) m_fVelX = m_fSpeed;

	//대쉬
	if (m_pInput->KeyDown(eKey::DOWN))
	{
		if (m_pInput->KeyPress(eKey::LEFT) && !m_bUseDash)
		{
			m_fDashDir = -1.f;
			m_bUseDash = true;
			m_bIsInvincible = true;
		}
		else if (m_pInput->KeyPress(eKey::RIGHT) && !m_bUseDash)
		{
			m_fDashDir = 1.f;
			m_bUseDash = true;
			m_bIsInvincible = true;
		}
	}
	//공격 
	if (!m_bAttacking && m_pInput->KeyDown(eKey::ATTACK_LEFT))
	{
		SetAttackDir();
		TryAttack();
		TryParry();
		m_bAttacking = true;
	}
	//공격 - 오브젝트 던지기
	if (!m_bAttacking && m_pInput->KeyDown(eKey::ATTACK_RIGHT))
	{
		SetAttackDir();
		m_bAttacking = true;
	}
}

void CPlayer::SetAttackDir()
{
	//마우스의 위치 - 플레이어의 위치 정규화 이후 방향 값을 속도에 곱한다
	float fDX = m_pInput->GetMouseX() - m_tInfo.fX;
	float fDY = m_pInput->GetMouseY() - m_tInfo.fY;
	float fLen = sqrtf(fDX * fDX + fDY * fDY);
	//방향 정규화 
	m_fAttackDirX = fDX / fLen;
	m_fAttackDirY = fDY / fLen;
}

void CPlayer::TryAttack()
{
	SetAttackDir();
	CObj* pAttack = CAbstractFactory<CAttack>::Create();
	pAttack->SetDir(m_fAttackDirX, m_fAttackDirY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_ATTACK, pAttack);
}

void CPlayer::TryParry()
{
	//패링 오브젝트만 생성하고 플레이어 추척은 패링 클래스에서 관리
	SetAttackDir();
	CObj* pParry = CAbstractFactory<CParry>::Create();
	pParry->SetDir(m_fAttackDirX, m_fAttackDirY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_PARRY, pParry);
}

std::unique_ptr<ISnapshot> CPlayer::SaveSnapshot() const
{
	auto snap = std::make_unique<PlayerSnapshot>();

	snap->fX = m_tInfo.fX;
	snap->fY = m_tInfo.fY;
	//속도
	snap->fVelX = m_fVelX;
	snap->fVelY = m_fVelY;
	//방향, 시선
	snap->fDirX = m_fDirX;
	snap->fDirY = m_fDirY;
	//생사
	snap->bDead = m_bDead;
	//애니메이션 상태
	snap->eAnimState = m_eAnim;
	//땅, 벽
	snap->bOnGround = m_bOnGround;
	snap->bAttachWall = m_bAttachWall;
	//대쉬
	snap->bUseDash = m_bUseDash;
	snap->fDashCoolTime = m_fDashCoolTime;
	snap->fDashRemainTime = m_fDashDuration;
	//공격
	snap->bAttacking = m_bAttacking;
	snap->fAttackCoolTime = m_fAttackCoolTime;
	snap->fAttackRemainTime = m_fAttackDuration;

	return snap;
}

void CPlayer::LoadSnapshot(const ISnapshot& snapshot)
{
	auto& snap = static_cast<const PlayerSnapshot&>(snapshot);

	//위치
	m_tInfo.fX = snap.fX;
	m_tInfo.fY = snap.fY;
	//속도
	m_fVelX = snap.fVelX;
	m_fVelY = snap.fVelY;
	//방향 시선
	m_fDirX = snap.fDirX;
	m_fDirY = snap.fDirY;
	//생사
	m_bDead = snap.bDead;
	//애니메이션 상태
	m_eAnim = snap.eAnimState;
	//땅, 벽
	m_bOnGround = snap.bOnGround;
	m_bAttachWall = snap.bAttachWall;
	//대쉬
	m_bUseDash = snap.bUseDash;
	m_fDashCoolTime = snap.fDashCoolTime;
	m_fDashDuration = snap.fDashRemainTime;
	//공격
	m_bAttacking = snap.bAttacking;
	m_fAttackCoolTime = snap.fAttackCoolTime;
	m_fAttackDuration = snap.fAttackRemainTime;
}

PlayerSnapshot CPlayer::SaveSnapshotValue() const
{
	PlayerSnapshot snap{};

	snap.fX = m_tInfo.fX;
	snap.fY = m_tInfo.fY;
	//속도
	snap.fVelX = m_fVelX;
	snap.fVelY = m_fVelY;
	//방향, 시선
	snap.fDirX = m_fDirX;
	snap.fDirY = m_fDirY;
	//생사
	snap.bDead = m_bDead;
	//애니메이션 상태
	snap.eAnimState = m_eAnim;
	//땅, 벽
	snap.bOnGround = m_bOnGround;
	snap.bAttachWall = m_bAttachWall;
	//대쉬
	snap.bUseDash = m_bUseDash;
	snap.fDashCoolTime = m_fDashCoolTime;
	snap.fDashRemainTime = m_fDashDuration;
	//공격
	snap.bAttacking = m_bAttacking;
	snap.fAttackCoolTime = m_fAttackCoolTime;
	snap.fAttackRemainTime = m_fAttackDuration;

	return snap;
}

void CPlayer::LoadSnapshotValue(const PlayerSnapshot& snap)
{
	//위치
	m_tInfo.fX = snap.fX;
	m_tInfo.fY = snap.fY;
	//속도
	m_fVelX = snap.fVelX;
	m_fVelY = snap.fVelY;
	//방향 시선
	m_fDirX = snap.fDirX;
	m_fDirY = snap.fDirY;
	//생사
	m_bDead = snap.bDead;
	//애니메이션 상태
	m_eAnim = snap.eAnimState;
	//땅, 벽
	m_bOnGround = snap.bOnGround;
	m_bAttachWall = snap.bAttachWall;
	//대쉬
	m_bUseDash = snap.bUseDash;
	m_fDashCoolTime = snap.fDashCoolTime;
	m_fDashDuration = snap.fDashRemainTime;
	//공격
	m_bAttacking = snap.bAttacking;
	m_fAttackCoolTime = snap.fAttackCoolTime;
	m_fAttackDuration = snap.fAttackRemainTime;
	//프레임 꼬임 방지
	__super::Update_Rect();
}
