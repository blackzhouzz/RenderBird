#pragma once
#include "core_private.h"
#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;

namespace Core
{
	class IntervalTime
	{
	public:
		void Begin()
		{
			m_beginTime = high_resolution_clock::now();
		}

		int64 End()
		{
			high_resolution_clock::time_point endTime = high_resolution_clock::now();
			m_timeInterval = std::chrono::duration_cast<milliseconds>(endTime - m_beginTime);
			return m_timeInterval.count();
		}

		milliseconds Interval()const
		{
			return m_timeInterval;
		}
		int64 GetIntervalInMilliseconds()const
		{
			return m_timeInterval.count();
		}
		int64 GetIntervalInSeconds()const
		{
			return m_timeInterval.count() / 1000;
		}
	private:
		high_resolution_clock::time_point m_beginTime;
		milliseconds m_timeInterval;

	};
}