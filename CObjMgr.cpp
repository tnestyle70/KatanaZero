#include "pch.h"
#include "CObjMgr.h"
#include "CCollisionMgr.h"

//static 전역으로 m_pInstance를 선언해서 프로그램이 종료될 때까지 사용을 한다.
CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
}

void CObjMgr::Add_Object(eObjID eID, CObj* pObject)
{
	//예외 처리
	if (eID >= OBJ_END || pObject == nullptr)
		return;
	m_listObj[eID].push_back(pObject);
}

CObj* CObjMgr::Get_Object(eObjID eID)
{
	//예외 처리
	if (eID >= OBJ_END)
		return nullptr;
	return m_listObj[eID].front();
}

list<CObj*>* CObjMgr::Get_Object_List(eObjID eID)
{
	if (eID >= OBJ_END)
	{
		return nullptr;
	}
	return &m_listObj[eID];
}

void CObjMgr::Update(float fDelatTime)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_listObj[i].begin();
			iter != m_listObj[i].end();)
		{
			int iResult = (*iter)->Update(fDelatTime);
			if (iResult == DEAD)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_listObj[i].erase(iter);
			}
			else
				iter++;
		}
	}
}

void CObjMgr::Late_Update(float fDelatTime)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			pObj->Late_Update(fDelatTime);
		}
	}
	//CCollisionMgr::RectCollide(m_listObj[OBJ_ENEMY_SHOTGUN], m_listObj[OBJ_BULLET]);
	//CCollisionMgr::RectCollide(m_listObj[OBJ_MONSTER], m_listObj[OBJ_MOUSE]);
}

void CObjMgr::Render(HDC hDC)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& pObj : m_listObj[i])
		{
			pObj->Render(hDC);
		}
	}
}

void CObjMgr::Release()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for_each(m_listObj[i].begin(), m_listObj[i].end(), Safe_Delete<CObj*>);
		m_listObj[i].clear();
	}
}

CObj* CObjMgr::Get_Target(eObjID eID, CObj* pObj)
{
	if (m_listObj[eID].empty())
		return nullptr;
	CObj* pTarget = nullptr;

	float fDistance(0.f);
	float fDiagnoal(0.f);

	for (auto& pTargetElement : m_listObj[eID])
	{
		float fDX = pObj->GetInfo()->fX - pTargetElement->GetInfo()->fX;
		float fDY = pObj->GetInfo()->fY - pTargetElement->GetInfo()->fY;
		fDiagnoal = sqrtf(fDX * fDX + fDY * fDY);
		if (!(pTarget) || fDistance > fDiagnoal)
		{
			pTarget = pTargetElement;
			fDistance = fDiagnoal;
			break;
			//break를 하게 되면 그 방향으로 쭉 앞으로 나아가게 된다.
		}
	}
	return pTarget;
}
