#pragma once

#include <vector>

#include "LTimer.h"
#include "Renderer.h"
#include "InputManager.h"
#include "EventListener.h"

#include "Grid.h"
#include "Player.h"
#include "Enemy.h"
#include "ThreadPool.h"

// The game objct that manages the game loop
class Game : public EventListener
{
public:
	Game();														// Constructor
	~Game();													// Standard destructor

	bool					init();								// Standard initialiser
	void					destroy();							// Function to destroy all game objects

	void					update();							// Standard update
	void					render();							// Standard render
	void					loop();								// Standard game loop

	void					onEvent(EventListener::Event);		// Event Listener for user inout

private:
	InputManager			m_inputManager;						// Object to control user input
	Renderer				m_renderer;							// Object to control the rendering

	unsigned int			m_lastTime;							// Time of last update
	bool					m_quit;								// Boolean to quit out of the game
	int						m_numTiles;							// The number of rows / columns
	int						m_numEnemies;

	Grid*					m_grid = nullptr;					// Pointer to grid object
	Player*					m_player = nullptr;					// Pointer to player object
	std::vector<Enemy*>		m_enemies;							// The list of enemies

	ThreadPool*				m_pool = nullptr;					// The list of threads we use to run our Astar

	void					resetBoard();						// Function to reset the board
};

