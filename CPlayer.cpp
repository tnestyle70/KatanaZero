#include "pch.h"
#include "CPlayer.h"
#include  "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CParry.h"
#include "CInputManager.h"

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
}

int CPlayer::Update(float fDeltaTime)
{
	if (m_bDead)
		return DEAD;

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
	m_fVelY += m_fGravity * fDeltaTime;

	UpdateDash(fDeltaTime);

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

void CPlayer::ResolveTileCollision()
{
	if (m_tRect.bottom >= WINCY)
	{
		m_tInfo.fY = WINCY - m_tInfo.fCY * 0.5f;
		m_fVelY = 0.f; //땅에 닿았을 때 프레임마다 중력 0 초기화 
		m_bOnGround = true;
	}
	
	//충돌 처리 
	if (m_tRect.left <= 0)
	{
		m_tInfo.fX = m_tInfo.fCX * 0.5f;
		m_bAttachWall = true;
	}
	if (m_tRect.right >= WINCX)
	{
		m_tInfo.fX = WINCX - m_tInfo.fCX * 0.5f;
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
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
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
		TryParry();
		m_bAttacking = true;
	}
	//공격 - 오브젝트 던지기
	if (!m_bAttacking && m_pInput->KeyDown(eKey::ATTACK_RIGHT))
	{
		SetAttackDir();
		m_bAttacking = true;
	}
	//타임 슬로우
	if (m_pInput->KeyDown(eKey::TIME_SLOW))
	{
		
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

void CPlayer::TryParry()
{
	//패링 오브젝트만 생성하고 플레이어 추척은 패링 클래스에서 관리
	SetAttackDir();
	CObj* pParry = CAbstractFactory<CParry>::Create();
	pParry->SetDir(m_fAttackDirX, m_fAttackDirY);
	CObjMgr::Get_Instance()->Add_Object(OBJ_PARRY, pParry);
}

CPlayerMemento CPlayer::SaveToMemento() const
{
	CPlayerMemento mem{};
	//위치
	mem.fX = m_tInfo.fX;
	mem.fY = m_tInfo.fY;
	//속도
	mem.fVelX = m_fVelX;
	mem.fVelY = m_fVelY;
	//방향, 시선
	mem.fDirX = m_fDirX;
	mem.fDirY = m_fDirY;
	//생사
	mem.bDead = m_bDead;
	//애니메이션 상태
	mem.iAnimState = m_eAnim;
	//땅, 벽
	mem.bOnGround = m_bOnGround;
	mem.bAttackWall = m_bAttachWall;
	//대쉬
	mem.bUseDash = m_bUseDash;
	mem.fDashCoolTime = m_fDashCoolTime;
	mem.fDashRemainTime = m_fDashDuration;
	//공격
	mem.bAttacking = m_bAttacking;
	mem.fAttackCoolTime = m_fAttackCoolTime;
	mem.fAttackRemainTime = m_fAttackDuration;
	//시간 조절
	//mem.fSlowGauge = 

	return CPlayerMemento();
}

void CPlayer::RestoreFromMemento(const CPlayerMemento& memento)
{
}
