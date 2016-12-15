#pragma once

#include "BasicTypes.h"
#include "Renderer.h"
#include <unordered_map>

// Basic Player class
class Player
{
public:
	Player(float width, float height, Point2D pos, std::pair<int, int> index);		// Constructor
	~Player();																		// Standard Destructor

	void					draw(Renderer& renderer);								// Standard Draw

	std::pair<int, int>		getIndex();												// Returns the index of the player on the grid

	void					setPosition(Point2D pos);								// Sets the position of the player

	void					moveUp();												// Moves the up a single tile
	void					moveDown();												// Moves the down a single tile
	void					moveLeft();												// Moves the left a single tile
	void					moveRight();											// Moves the right a single tile

private:
	Rect					m_rect;													// The rectangle object we draw
	Colour					m_colour;												// The colour of the rectangle
	std::pair<int, int>		m_index;												// The index of the player on the grid

	//SDL_mutex*				m_lock;
};