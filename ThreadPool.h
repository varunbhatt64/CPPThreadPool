#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

typedef void(*callback_function)(const std::string& str);

class ThreadPool
{
public:
	ThreadPool(callback_function callback, int numberOfThreads = std::thread::hardware_concurrency());
	~ThreadPool();
	void QueueWork(const std::string& buffer);

private:
	callback_function CallBack;
	std::condition_variable_any workQueueConditionVariable;
	std::vector<std::thread> threads;
	std::mutex workQueueMutex;
	std::queue<std::string> workQueue;
	bool done;
	void DoWork();
	void Process(const std::string& buffer);
};

