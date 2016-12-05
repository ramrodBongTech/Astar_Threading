#pragma once

// Used as an interface for the event dispatcher
// Objects wanting to receive events must inherit from this

class EventListener
{
public:

	// List of possible events
	enum Event 
	{
		ONE,						// One key
		TWO,						// Two key
		THREE,						// Three key
		UP,							// Up key
		DOWN,						// Down key
		LEFT,						// Left key
		RIGHT,						// Right key
		QUIT,						// Quit key
		R,							// R key
		RETURN						// Return key
	};

	virtual void onEvent(Event) = 0;
};

