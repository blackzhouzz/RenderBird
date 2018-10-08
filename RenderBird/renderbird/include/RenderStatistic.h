#pragma once
#include "RenderbirdPrivate.h"
#include <atomic> 
#include "timer.h"

namespace RenderBird
{
	class RenderStatistic
	{
	public:
		static std::atomic<uint64> m_numRaySceneIntersect;
		static std::atomic<uint64> m_numRayBVHIntersect;
		static std::atomic<uint64> m_numRayTriangleIntersect;
		static std::atomic<uint64> m_numSampleProcessed;
		static IntervalTime m_timer;
		static void Print();
		static nanoseconds tmpTime;
		static std::atomic<uint64> m_maxDepth;
	};
}