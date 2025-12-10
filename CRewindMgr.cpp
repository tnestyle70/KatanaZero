#include "pch.h"
#include "CRewindMgr.h"

CRewindMgr* CRewindMgr::m_pInstance = nullptr;

CRewindMgr::CRewindMgr() : m_bRewinding(false)
{
}

void CRewindMgr::StoreFrame(CObjMgr& mgr)
{
	Frame frame;
	const size_t MAX_FRAMES = 8000;
	for (int i = 0; i < OBJ_END; ++i)
	{
		auto* pList = mgr.Get_Instance()->Get_Object_List((eObjID)i);
		if (!pList) continue;
		for (auto& obj : *pList)
		{
			if (auto* mem = dynamic_cast<IMemento*>(obj))
			{
				auto snap = mem->SaveSnapshot();
				if (snap)
				{
					frame.emplace(obj, std::move(snap));
				}
			}
		}
	}
	m_dRewindFrames.push_back(std::move(frame));
	if (m_dRewindFrames.size() > MAX_FRAMES)
	{
		m_dRewindFrames.pop_front();
	}
}

void CRewindMgr::RewindFrame(CObjMgr& mgr)
{
	if (m_dRewindFrames.empty())
	{
		return;
	}
	Frame& frame = m_dRewindFrames.back();

	for (auto& [obj, snap] : frame)
	{
		if (!obj || !snap) continue;
		if (auto* mem = dynamic_cast<IMemento*>(obj))
		{
			mem->LoadSnapshot(*snap);
		}
	}
	m_dRewindFrames.pop_back();
}

