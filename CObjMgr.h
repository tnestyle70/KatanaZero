#pragma once
#include "CObj.h"

//CObjMgr를 싱글톤 + 메디에이터 패턴과 결합해서 사용을 한다.
class CObjMgr
{
private:
	CObjMgr();
	//복사 생성 막기, 복사 생성 시도하면 delete로 바로 지워버리기
	CObjMgr(const CObjMgr& rhs) = delete;
	//대입 연산자를 사용한 복사 생성 동일하게 막기
	CObjMgr& operator=(CObjMgr& rObj) = delete;
	~CObjMgr();
public:
	void Add_Object(eObjID eID, CObj* pObject);
	CObj* Get_Object(eObjID eID);
	list<CObj*>* Get_Object_List(eObjID eID);
	void Update(float fDelatTime);
	void Late_Update(float fDelatTime);
	void Update_Rect();
	void Render(HDC hDC);
	void Release();
public:
	//Bullet에서 선언한 Find 몬스터 구현
	CObj* Get_Target(eObjID eID, CObj* pTarget);
public:
	static CObjMgr* Get_Instance()
	{
		//처음 호출했을 경우 생성
		if (nullptr == m_pInstance)
		{
			m_pInstance = new CObjMgr;
		}
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	//전역에 하나 존재하는 Inst 생성
	static CObjMgr* m_pInstance;
	//Main 게임에서 생성한 objlist를 Mgr에서 관리
	list<CObj*> m_listObj[OBJ_END];
};

