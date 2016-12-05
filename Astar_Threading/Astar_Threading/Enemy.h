#pragma once

#include "BasicTypes.h"
#include "Renderer.h"
#include <unordered_map>

// Basic Enemy class
class Enemy
{
public:
	Enemy();																			// Standard Constructor
	Enemy(float width, float height, Point2D pos, std::pair<int, int> index);			// Overloaded Constructor
	~Enemy();																			// Standard Destructor

	void					draw(Renderer& renderer);									// Standard Draw function

	std::pair<int, int>		getIndex();													// Returns the index of the enemy on the grid

	void					setPosition(Point2D pos);									// Sets the position of the enemy

private:
	Rect					m_rect;														// The rectangle object we draw
	Colour					m_colour;													// The colour object
	std::pair<int, int>		m_index;													// The index of the enemy
};