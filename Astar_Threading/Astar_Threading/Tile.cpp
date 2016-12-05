#include "stdafx.h"
#include "Tile.h"

Tile::Tile(float width, float height, Point2D pos, std::pair<int, int> index) :
	m_fScore(0),
	m_heuristic(0),
	m_costSoFar(0),
	m_wall(false),
	m_rect(Rect(pos, Size2D(width, height))),
	m_index(index)
{}

Tile::~Tile(){}

float Tile::getFScore(){ return m_fScore; }
float Tile::getHeuristic(){ return m_heuristic; }
float Tile::getCostSoFar(){ return m_costSoFar; }
Point2D Tile::getPosition(){ return m_rect.pos; }
std::pair<int, int>	Tile::getIndex(){ return m_index; }
Tile* Tile::getPrevious(){ return m_previous; }
bool Tile::isWall() { return m_wall; }
bool Tile::isMarked(){ return m_marked; }

void Tile::setColor(Colour c){ m_colour = c; }
void Tile::setFScore(float fScore){ m_fScore = fScore; }
void Tile::setHeuristic(float heuristic){ m_heuristic = heuristic; }
void Tile::setCostSoFar(float csf){ m_costSoFar = csf; }
void Tile::setMarked(bool marked){ m_marked = marked; }
void Tile::setPrevious(Tile* t){ m_previous = t; }

void Tile::setWall()
{ 
	m_wall = true; 
	m_colour = Colour(0, 0, 255);
}

void Tile::draw(Renderer& renderer){ renderer.drawRect(m_rect, m_colour); }