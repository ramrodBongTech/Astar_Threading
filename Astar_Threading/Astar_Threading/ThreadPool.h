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
	//static ThreadPool* getInstance();

	void addTask(std::function<void*()> func);			// Function to add a task to the list
	std::function<void*()> consume();					// Function for the thread to consume a task

	SDL_mutex* getMutex();								// Gets the mutex
	SDL_sem* getSemaphore();							// Gets the semaphore

private:
	std::vector<SDL_Thread*>			workers;		// Vector of threads that will consume tasks

	std::deque<std::function<void*()>>	tasks;			// The tasks we want to do

	SDL_mutex*							mutex;			// Mutex lock
	SDL_sem*							semaphore;		// Semaphore

	int									numThreads;		// Number of threads in the pool
	int									numTasks;		// Number of tasks available
};

//static ThreadPool* instance = 0;

// Static function for the thread to run
// Runs an infinite loop waiting for the semaphore to signal
// Gets a task from the list and runs it
static int work(void* ptr)
{
	//ThreadPool* tp = ThreadPool::getInstance();
	ThreadPool* tp = (ThreadPool*)ptr;
	SDL_sem* semaphore = tp->getSemaphore();

	// look for a work item
	while (true)
	{ // if there are none wait for notification
		SDL_SemWait(semaphore);
		auto& task = tp->consume();
		task();
	}
}