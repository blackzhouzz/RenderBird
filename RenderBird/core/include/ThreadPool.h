#pragma once
#include "CorePrivate.h"
#include <condition_variable>
#include <unordered_map>
#include <functional>
#include <atomic>
#include <memory>
#include <thread>
#include <vector>
#include <deque>
#include <mutex>

namespace Core
{
	class TaskGroup
	{
		typedef std::function<void(uint32, uint32, uint32)> TaskFunc;
		typedef std::function<void()> Finisher;

		TaskFunc _func;
		Finisher _finisher;

		std::exception_ptr _exceptionPtr;
		std::atomic<uint32> _startedSubTasks;
		std::atomic<uint32> _finishedSubTasks;
		uint32 _numSubTasks;

		std::mutex _waitMutex;
		std::condition_variable _waitCond;
		std::atomic<bool> _done, _abort;

		void finish()
		{
			if (_finisher && !_abort)
				_finisher();

			std::unique_lock<std::mutex> lock(_waitMutex);
			_done = true;
			_waitCond.notify_all();
		}

	public:
		TaskGroup(TaskFunc func, Finisher finisher, uint32 numSubTasks)
			: _func(std::move(func)),
			_finisher(std::move(finisher)),
			_startedSubTasks(0),
			_finishedSubTasks(0),
			_numSubTasks(numSubTasks),
			_done(false),
			_abort(false)
		{
		}

		void run(uint32 threadId, uint32 taskId)
		{
			try {
				_func(taskId, _numSubTasks, threadId);
			}
			catch (...) {
				_exceptionPtr = std::current_exception();
			}

			uint32 num = ++_finishedSubTasks;
			if (num == _numSubTasks || (_abort && num == _startedSubTasks))
				finish();
		}

		void wait()
		{
			std::unique_lock<std::mutex> lock(_waitMutex);
			_waitCond.wait(lock, [this] {return _done == true; });

			if (_exceptionPtr)
				std::rethrow_exception(_exceptionPtr);
		}

		void abort()
		{
			std::unique_lock<std::mutex> lock(_waitMutex);
			_abort = true;
			_done = _done || _startedSubTasks == 0;
			_waitCond.notify_all();
		}

		bool isAborting() const
		{
			return _abort;
		}

		bool isDone() const
		{
			return _done;
		}

		uint32 startSubTask()
		{
			return _startedSubTasks++;
		}

		uint32 numSubTasks() const
		{
			return _numSubTasks;
		}
	};


	class ThreadPool
	{
		typedef std::function<void(uint32, uint32, uint32)> TaskFunc;
		typedef std::function<void()> Finisher;

		uint32 _threadCount;
		std::vector<std::unique_ptr<std::thread>> _workers;
		std::atomic<bool> _terminateFlag;

		std::deque<std::shared_ptr<TaskGroup>> _tasks;
		std::mutex _taskMutex;
		std::condition_variable _taskCond;

		std::unordered_map<std::thread::id, uint32> _idToNumericId;

		std::shared_ptr<TaskGroup> acquireTask(uint32 &subTaskId);
		void runWorker(uint32 threadId);
		void startThreads();

	public:
		ThreadPool(uint32 threadCount);
		~ThreadPool();

		void yield(TaskGroup &wait);

		void reset();
		void stop();

		std::shared_ptr<TaskGroup> enqueue(TaskFunc func, int numSubtasks = 1,
			Finisher finisher = Finisher());

		uint32 threadCount() const
		{
			return _threadCount;
		}
	};
}
