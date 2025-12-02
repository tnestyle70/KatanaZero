#include "pch.h"
#include "CPlayer.h"
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
	m_fGravity = 1000.f;
	m_fJumpPower = 600.f;
	m_bOnGround = false;
	m_bAttachWall = false;
	//대쉬용 변수
	m_bUseDash = false;
	m_fDashDir = 0.f;
	m_fDashCoolTime = 0.f;
	m_fDashDuration = DASH_DURATION;
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
	/*
	//충돌 처리 
	if (m_tRect.left <= 0)
	{
		m_tInfo.fX = WINCX - m_tInfo.fCX * 0.5f;
		m_bAttachWall = true;
	}
	if (m_tRect.right >= WINCX)
	{
		m_tInfo.fX = m_tRect.right - m_tInfo.fCX * 0.5f;
		m_bAttachWall = true;
	}
	*/
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
	//대쉬 쿨타임 디버깅
	WCHAR cBuf[64];
	swprintf_s(cBuf, L"마우스 좌표 : %.2f", m_pInput->GetMouseX());
	TextOutW(hDC, 10, 10, cBuf, lstrlenW(cBuf));
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
	if(m_pInput->KeyPress(eKey::LEFT))
	{
		m_fVelX = -m_fSpeed;
	}
	if (m_pInput->KeyPress(eKey::RIGHT))
	{
		m_fVelX = m_fSpeed;
	}
	//대쉬
	if (m_pInput->KeyPress(eKey::DOWN))
	{
		if (m_pInput->KeyPress(eKey::LEFT))
		{
			m_fDashDir = -1.f;
			m_bUseDash = true;
		}
		else if (m_pInput->KeyPress(eKey::RIGHT))
		{
			m_fDashDir = 1.f;
			m_bUseDash = true;
		}
	}
	//공격
	if (m_pInput->KeyPress(eKey::ATTACK_LEFT) ||
		m_pInput->KeyPress(eKey::ATTACK_RIGHT))
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
