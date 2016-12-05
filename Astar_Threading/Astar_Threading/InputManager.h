#pragma once

#define SDL_MAIN_HANDLED
#ifdef __APPLE__
#include "SDL2/SDL.h"
#elif defined(_WIN64) || defined(_WIN32)
#include "SDL.h"
#endif


#include<map>
#include<vector>

#include "EventListener.h"

// Converts input into logical events
class InputManager
{
	// Dictionary holding a list of listener objects for each event type
	std::map<
		EventListener::Event,
		std::vector<EventListener*>*							// Pointer to vector of event listeners
	> listeners;

public:
	InputManager();												// Standard Constructor
	~InputManager();											// Standard Destructor

	void AddListener(EventListener::Event, EventListener*);		// Function for adding event listeners
	void Dispatch(EventListener::Event);						// Sends the events to interested objects

	void ProcessInput();										// Genereate events
};

