#pragma once
#include "renderbird_private.h"
#include <atomic> 
#include "timer.h"

namespace RenderBird
{
	class RenderStatistic
	{
	public:
		static std::atomic<uint64> m_numRayIntersect;
		static IntervalTime m_timer;
	};
}