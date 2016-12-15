#include "stdafx.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool()
	:
	mutex(SDL_CreateMutex()),
	semaphore(SDL_CreateSemaphore(0)),
	numThreads(7),
	numTasks(0)
{
	for (int i = 0; i < numThreads; ++i)
		workers.push_back(SDL_CreateThread(work, "Worker", (void*)(i + 1)));
}

ThreadPool::~ThreadPool()
{
	for (size_t i = 0; i < numThreads; ++i)
		SDL_DetachThread(workers[i]);
}

ThreadPool* ThreadPool::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ThreadPool();
	}
	return instance;
}

void ThreadPool::addTask(std::function<void*()> func)
{
	// get lock
	SDL_LockMutex(mutex);
	// add the task
	tasks.push_back(func);
	// increment number of tasks
	numTasks++;
	// unlock 
	SDL_UnlockMutex(mutex);
	// wake up threads
	SDL_SemPost(semaphore);
}

std::function<void*()> ThreadPool::consume()
{
	// get lock
	SDL_LockMutex(mutex);
	// get the task
	auto task = tasks.front();
	// pop the task
	tasks.pop_front();
	// decrement number of tasks
	numTasks--;
	// unlock 
	SDL_UnlockMutex(mutex);
	// return the task
	return task;
}

SDL_mutex* ThreadPool::getMutex(){ return mutex; }

SDL_sem* ThreadPool::getSemaphore(){ return semaphore; }
