#include "ThreadPool.h"

ThreadPool::ThreadPool(callback_function callback, int numberOfThreads) : done(false)
{
	CallBack = callback;
	if (numberOfThreads == 0) {
		numberOfThreads = 1;
	}

	for (size_t i = 0; i < numberOfThreads; i++)
	{
		threads.push_back(std::thread(&ThreadPool::DoWork, this));
	}
}

ThreadPool::~ThreadPool()
{
	done = true;

	workQueueConditionVariable.notify_all();
	for (auto& thread : threads)
	{
		if (thread.joinable())
			thread.join();
	}
}

void ThreadPool::QueueWork(const std::string& buffer)
{
	std::lock_guard<std::mutex> g(workQueueMutex);
	workQueue.push(buffer);
	workQueueConditionVariable.notify_one();
}

void ThreadPool::DoWork()
{
	while (!done)
	{
		std::string buffer;
		{
			std::unique_lock<std::mutex> g(workQueueMutex);
			workQueueConditionVariable.wait(g, [&] {
				return (!workQueue.empty() || done);
				});

			buffer = workQueue.front();
			workQueue.pop();
		}

		Process(buffer);
	}
}

void ThreadPool::Process(const std::string& buffer)
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	if (CallBack)
		CallBack(buffer);
}
