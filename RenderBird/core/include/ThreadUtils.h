#pragma once
#include "CorePrivate.h"
#include "ThreadPool.h"
#include "MathCore.h"

#include <thread>
#if _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace Core
{
	namespace ThreadUtils 
	{
		extern ThreadPool *pool;

		inline uint32 idealThreadCount()
		{
			// std::thread::hardware_concurrency support is not great, so let's try
			// native APIs first
#if _WIN32
			SYSTEM_INFO info;
			GetSystemInfo(&info);
			if (info.dwNumberOfProcessors > 0)
				return info.dwNumberOfProcessors;
#else
			int nprocs = sysconf(_SC_NPROCESSORS_ONLN);
			if (nprocs > 0)
				return nprocs;
#endif
			unsigned n = std::thread::hardware_concurrency();
			if (n > 0)
				return n;

			// All attempts failed. Let's take a guess
			return 4;
		}

		inline void startThreads(int numThreads)
		{
			pool = new ThreadPool(numThreads);
		}

		inline void parallelFor(uint32 start, uint32 end, uint32 partitions, std::function<void(uint32)> func)
		{
			auto taskRun = [&func, start, end](uint32 idx, uint32 num, uint32 /*threadId*/)
			{
				uint32 span = (end - start + num - 1) / num;
				uint32 iStart = start + span * idx;
				uint32 iEnd = Min(iStart + span, end);
				for (uint32 i = iStart; i < iEnd; ++i)
					func(i);
			};
			if (partitions == 1)
				taskRun(0, 1, 0);
			else
				pool->yield(*pool->enqueue(taskRun, partitions));
		}

	}
}