#include "render_statistic.h"

namespace RenderBird
{
	std::atomic<uint64> RenderStatistic::m_numRayIntersect(0);
	IntervalTime RenderStatistic::m_timer;
}