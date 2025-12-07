#pragma once
#include "Define.h"
#include <memory>

//모든 스냅샷의 공통 베이스
struct ISnapshot
{
	virtual ~ISnapshot() = default;
};
//메멘토 기능 인터페이스 
class IMemento
{
public:
	virtual std::unique_ptr<ISnapshot> SaveSnapshot() const = 0;
	virtual void LoadSnapshot(const ISnapshot& snap) PURE;
	virtual ~IMemento() = default;
};