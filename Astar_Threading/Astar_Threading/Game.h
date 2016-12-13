#pragma once

#include <vector>

#include "LTimer.h"
#include "Renderer.h"
#include "InputManager.h"
#include "EventListener.h"

#include "Grid.h"
#include "Player.h"
#include "Enemy.h"

// The game objct that manages the game loop
class Game : public EventListener
{
public:
	Game();												// Constructor
	~Game();											// Standard destructor

	bool			init();								// Standard initialiser
	void			destroy();							// Function to destroy all game objects

	void			update();							// Standard update
	void			render();							// Standard render
	void			loop();								// Standard game loop

	void			onEvent(EventListener::Event);		// Event Listener for user inout

private:
	InputManager	m_inputManager;						// Object to control user input
	Renderer		m_renderer;							// Object to control the rendering

	unsigned int	m_lastTime;							// Time of last update
	bool			m_quit;								// Boolean to quit out of the game
	int				m_numTiles;							// The number of rows / columns

	SDL_Thread*		m_thread1 = nullptr;
	SDL_Thread*		m_thread2 = nullptr;

	Grid*			m_grid = nullptr;					// Pointer to grid object
	Player*			m_player = nullptr;					// Pointer to player object
	Enemy*			m_enemy = nullptr;					// Pointer to enemy object
	Enemy*			en2 = nullptr;						// Second enemy to test second thread

	void			resetBoard();						// Function to reset the board
	void			Astar(Tile* start, Tile* end);		// Function to run A* Algorithim	
	int				getCost(Tile* t1, Tile* t2);		// Function to get the cost from one tile to another

	//int threadedAstar(void* ptr);
};

