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

	float				getFScore();											// Returns the total estimated cost to the end node
	float				getHeuristic();											// Returns the heuristic cost to the end node
	float				getCostSoFar();											// Returns the cost to get to this node
	bool				isWall();												// Returns weither the tile is a wall or not
	bool				isMarked();												// Returns weither the node is marked or not
	Point2D				getPosition();											// Returns the position of the tile
	std::pair<int, int>	getIndex();												// Returns the index of the tile on the grid
	Tile*				getPrevious();											// Returns the parent node

	void				setColor(Colour c);										// Sets the colour of this tile
	void				setFScore(float fScore);								// Sets the total estimated cost to the end node
	void				setHeuristic(float heuristic);							// Sets the heuristic cost to the end node
	void				setCostSoFar(float csf);								// Sets the cost to get to this node
	void				setMarked(bool marked);									// Sets the node to be marked
	void				setPrevious(Tile* t);									// Sets the parent node

	void				setWall();												// Sets this tile to be a wall

private:
	float				m_fScore;												// The total estimated cost to the end node
	float				m_heuristic;											// The heuristic cost to the end node
	float				m_costSoFar;											// The cost to get to this node
	bool				m_wall;													// To determine if it is a wall or not
	bool				m_marked;												// To determine if it is opened by the A* algorithm
	Rect				m_rect;													// The rectangle that will be drawn
	Colour				m_colour;												// The colour the rectangle will be
	std::pair<int, int>	m_index;												// The index of the tile on the grid
	Tile*				m_previous = nullptr;									// Pointer to the parent node
};
