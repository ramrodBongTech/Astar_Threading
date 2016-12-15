#pragma once

#include <SDL.h>
#include <thread>
#include <queue>
#include <functional>
#include <vector>

class ThreadPool 
{
public:
	ThreadPool();
	~ThreadPool();
	static ThreadPool* getInstance();

	void addTask(std::function<void*()> func);
	std::function<void*()> consume();

	SDL_mutex* getMutex();
	SDL_sem* getSemaphore();

private:
	// need to keep track of threads so we can join them
	std::vector<SDL_Thread*> workers;
	// the task queue
	std::deque<std::function<void*()>> tasks;
	// synchronization
	SDL_mutex* mutex;
	SDL_sem* semaphore;

	int numThreads;
	int numTasks;
};

static ThreadPool* instance = 0;

static int work(void* ptr)
{
	ThreadPool* tp = ThreadPool::getInstance();
	SDL_mutex* mutex = tp->getMutex();
	SDL_sem* semaphore = tp->getSemaphore();

	// look for a work item
	while (true)
	{ // if there are none wait for notification
		SDL_SemWait(semaphore);
		auto& task = tp->consume();
		task();
	}
}