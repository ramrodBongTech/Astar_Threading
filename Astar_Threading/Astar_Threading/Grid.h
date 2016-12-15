#pragma once

#include <SDL.h>
#include "Tile.h"
#include <vector>
#include <cmath>

// A class that creates a 2D vector of tiles
class Grid
{
public:
	Grid();
	Grid(float width, float height, int numTiles);									// Constructor
	~Grid();																		// Standard destructor

	void							draw(Renderer& renderer);						// Standard draw method

	Tile*							getTile(std::pair<int, int> index);				// Returns the tile at a specific index on the grid
	std::vector<Tile*>				getNeighbourNodes(std::pair<int, int> index);	// Returns a vector of tiles that are adjacent to a specific index

	void							resetBoard();									// Resets the board

private:
	std::vector<std::vector<Tile*>>	m_grid;											// The grid -> a 2D vector of tiles
	
	
};
