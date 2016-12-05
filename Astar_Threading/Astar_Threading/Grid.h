#pragma once

#include <SDL.h>
#include "Tile.h"
#include <vector>

// A class that creates a 2D vector of tiles
class Grid
{
public:
	Grid(float width, float height, int numTiles);									// Constructor
	~Grid();																		// Standard destructor

	void							draw(Renderer& renderer);						// Standard draw method

	Tile*							getTile(std::pair<int, int> index);				// Returns the tile at a specific index on the grid
	int								getNumTiles();									// Returns the number of rows in the grid
	std::vector<Tile*>				getNeighbourNodes(std::pair<int, int> index);	// Returns a vector of tiles that are adjacent to a specific index

	float							cost(Tile* t1, Tile* t2);						// Returns the cost between two tiles -> the distance from one to the other
	void							resetBoard();									// Resets the board

private:
	std::vector<std::vector<Tile*>>	m_grid;											// The grid -> a 2D vector of tiles
	
	
};
