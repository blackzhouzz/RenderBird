#include "render_statistic.h"
#include <iostream>
namespace RenderBird
{
	std::atomic<uint64> RenderStatistic::m_numRaySceneIntersect(0);
	std::atomic<uint64> RenderStatistic::m_numRayBVHIntersect(0);
	std::atomic<uint64> RenderStatistic::m_numRayTriangleIntersect(0);
	std::atomic<uint64> RenderStatistic::m_numSampleProcessed(0);
	std::atomic<uint64> RenderStatistic::m_maxDepth(0);
	IntervalTime RenderStatistic::m_timer;
	nanoseconds RenderStatistic::tmpTime(0);

	void RenderStatistic::Print()
	{
		std::cout << "Num ray scene is " << RenderStatistic::m_numRaySceneIntersect << std::endl;
		std::cout << "Num ray bvh is " << RenderStatistic::m_numRayBVHIntersect << std::endl;
		std::cout << "Num ray triangle is " << RenderStatistic::m_numRayTriangleIntersect << std::endl;
		std::cout << "Num sample processed is " << RenderStatistic::m_numSampleProcessed << std::endl;
		std::cout << "temp time" << RenderStatistic::tmpTime.count() * 0.000000001 << std::endl;
		std::cout << "Max depth" << RenderStatistic::m_maxDepth << std::endl;
	}
}