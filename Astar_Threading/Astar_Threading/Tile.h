#pragma once

#include <SDL.h>
#include "BasicTypes.h"
#include "Renderer.h"
#include <unordered_map>

// Tile class -> also used as the node for the A* algorithm
class Tile
{
public:
	Tile(float width, float height, Point2D pos, std::pair<int, int> index);	// Constructor
	~Tile();																	// Standard Destructor

	void draw(Renderer& renderer);												// Standard Draw function

	int					getFScore();											// Returns the total estimated cost to the end node
	bool				isWall();												// Returns weither the tile is a wall or not
	Point2D				getPosition();											// Returns the position of the tile
	std::pair<int, int>	getIndex();												// Returns the index of the tile on the grid

	void				setColor(Colour c);										// Sets the colour of this tile
	void				setFScore(float fScore);								// Sets the total estimated cost to the end node
	
	void				setWall();												// Sets this tile to be a wall

private:
	int					m_fScore;												// The total estimated cost to the end node
	bool				m_wall;													// To determine if it is a wall or not
	Rect				m_rect;													// The rectangle that will be drawn
	Colour				m_colour;												// The colour the rectangle will be
	std::pair<int, int>	m_index;												// The index of the tile on the grid
	
	SDL_mutex*			m_lock;													// Mutex to lock critical sections
};
