#include "stdafx.h"
#include "InputManager.h"

InputManager::InputManager(){}

InputManager::~InputManager(){}

void InputManager::AddListener(EventListener::Event evt, EventListener *listener)
{
	if (listeners.find(evt) == listeners.end()) 
		listeners[evt] = new std::vector<EventListener*>();

	listeners[evt]->push_back(listener);
}

void InputManager::Dispatch(EventListener::Event evt)
{
	if (listeners.find(evt) != listeners.end()) 
		for (auto const &listener : *listeners[evt]) 
			listener->onEvent(evt);					// Call on event for the listener
}

void InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type) 
		{
		case SDL_KEYDOWN:

			switch (e.key.keysym.sym) 
			{
			case SDLK_w:
				Dispatch(EventListener::Event::UP);
				break;

			case SDLK_s:
				Dispatch(EventListener::Event::DOWN);
				break;

			case SDLK_a:
				Dispatch(EventListener::Event::LEFT);
				break;

			case SDLK_d:
				Dispatch(EventListener::Event::RIGHT);
				break;

			}
			break;

		case SDL_KEYUP:
			switch (e.key.keysym.sym) 
			{
			case SDLK_1:
				Dispatch(EventListener::Event::ONE);
				break;

			case SDLK_2:
				Dispatch(EventListener::Event::TWO);
				break;

			case SDLK_3:
				Dispatch(EventListener::Event::THREE);
				break;

			case SDLK_r:
				Dispatch(EventListener::Event::R);
				break;

			case SDLK_RETURN:
				Dispatch(EventListener::Event::RETURN);
				break;

			}
			break;
			
		case SDL_QUIT:
			Dispatch(EventListener::Event::QUIT);
			break;

		default:
			break;
		}
	}
}