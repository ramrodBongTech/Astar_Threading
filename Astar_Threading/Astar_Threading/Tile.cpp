#include "stdafx.h"
#include "Tile.h"

Tile::Tile(float width, float height, Point2D pos, std::pair<int, int> index) :
	m_wall(false),
	m_rect(Rect(pos, Size2D(width, height))),
	m_index(index),
	m_lock(SDL_CreateMutex())
{}

Tile::~Tile()
{
	SDL_DestroyMutex(m_lock);
}

Point2D Tile::getPosition()
{
	SDL_LockMutex(m_lock);
	Point2D temp = m_rect.pos;
	SDL_UnlockMutex(m_lock);
	return temp;
}

std::pair<int, int>	Tile::getIndex()
{
	SDL_LockMutex(m_lock);
	std::pair<int, int> temp = m_index;
	SDL_UnlockMutex(m_lock);
	return temp;
}

bool Tile::isWall()
{
	SDL_LockMutex(m_lock);
	bool temp = m_wall;
	SDL_UnlockMutex(m_lock);
	return m_wall;
}

void Tile::setColor(Colour c)
{
	SDL_LockMutex(m_lock);
	m_colour = c;
	SDL_UnlockMutex(m_lock);
}

void Tile::setWall()
{ 
	m_wall = true; 
	m_colour = Colour(0, 0, 255);
}

void Tile::draw(Renderer& renderer)
{
	SDL_LockMutex(m_lock);
	renderer.drawRect(m_rect, m_colour);
	SDL_UnlockMutex(m_lock);
}