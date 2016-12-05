#include "stdafx.h"
#include "Grid.h"

Grid::Grid(float width, float height, int numTiles)
{
	float tileWidth = width / numTiles;
	float tileHeight = height / numTiles;

	for (int i = 0; i < numTiles; i++)
	{
		std::vector<Tile*> tiles;

		for (int j = 0; j < numTiles; j++)
		{
			// Fix if statement for exact board in spec
			if (j == numTiles / 5 && i < numTiles * 0.7 ||
				j == numTiles / 2 && i > numTiles / 5 ||
				j == numTiles * 0.8 && i < numTiles * 0.7)
			{
				Tile* t = new Tile(tileWidth, tileHeight, Point2D(j * tileWidth, i * tileHeight), std::pair<int, int>(i, j));
				t->setWall();
				tiles.push_back(t);
			}
			else
			{
				tiles.push_back(new Tile(tileWidth, tileHeight, Point2D(j * tileWidth, i * tileHeight), std::pair<int, int>(i, j)));

				if (i % 2 == 0)
				{
					if (j % 2 == 0)
						tiles[j]->setColor(Colour(0, 0, 0));
					else
						tiles[j]->setColor(Colour(255, 255, 255));
				}
				else
				{
					if (j % 2 != 0)
						tiles[j]->setColor(Colour(0, 0, 0));
					else
						tiles[j]->setColor(Colour(255, 255, 255));
				}
			}
		}

		m_grid.push_back(tiles);
	}

	int check = 0;
}

Grid::~Grid()
{
	for (int row = 0; row < m_grid.size(); row++)
	{
		for (int col = 0; col < m_grid[row].size(); col++)
		{
			delete m_grid[row][col];
			m_grid[row][col] = nullptr;
		}
	}
}

void Grid::draw(Renderer& renderer)
{
	for (int i = 0; i < m_grid.size(); i++)
		for (int j = 0; j < m_grid[i].size(); j++)
			m_grid[i][j]->draw(renderer);
}

Tile* Grid::getTile(std::pair<int, int> index)
{
	if (index.first >= 0 && index.first < m_grid.size() &&
		index.second >= 0 && index.second < m_grid.size())
	{

		if (m_grid[index.first][index.second]->isWall())
			return nullptr;
		else
			return m_grid[index.first][index.second];
	}
	else
		return nullptr;
}

int Grid::getNumTiles(){ return m_grid.size(); }

std::vector<Tile*> Grid::getNeighbourNodes(std::pair<int, int> index)
{
	std::vector<Tile*> _tiles;
	
	// North
	Tile* _t = getTile(std::pair<int, int>(index.first - 1, index.second));
	if (_t != nullptr && !_t->isWall())
		_tiles.push_back(_t);

	// North West
	/*_t = getTile(std::pair<int, int>(index.first - 1, index.second - 1));
	if (_t != nullptr && !_t->isWall())
		_tiles.push_back(_t);*/

	// West
	_t = getTile(std::pair<int, int>(index.first, index.second - 1));
	if (_t != nullptr && !_t->isWall())
		_tiles.push_back(_t);

	// South West
	/*_t = getTile(std::pair<int, int>(index.first + 1, index.second - 1));
	if (_t != nullptr && !_t->isWall())
		_tiles.push_back(_t);*/

	// South
	_t = getTile(std::pair<int, int>(index.first + 1, index.second));
	if (_t != nullptr && !_t->isWall())
		_tiles.push_back(_t);

	// South East
	/*_t = getTile(std::pair<int, int>(index.first + 1, index.second + 1));
	if (_t != nullptr && !_t->isWall())
		_tiles.push_back(_t);*/

	// East
	_t = getTile(std::pair<int, int>(index.first, index.second + 1));
	if (_t != nullptr && !_t->isWall())
		_tiles.push_back(_t);

	// North East
	/*_t = getTile(std::pair<int, int>(index.first - 1, index.second + 1));
	if (_t != nullptr && !_t->isWall())
		_tiles.push_back(_t);*/

	return _tiles;
}

float Grid::cost(Tile* t1, Tile* t2)
{
	//float c = (t2->getPosition().x - t1->getPosition().x) + (t2->getPosition().y - t1->getPosition().y);
	float n = std::sqrt(((t1->getPosition().x - t2->getPosition().x) * (t1->getPosition().x - t2->getPosition().x)) + ((t1->getPosition().y - t2->getPosition().y) * (t1->getPosition().y - t2->getPosition().y)));

	return n;
}

void Grid::resetBoard()
{
	for (int i = 0; i < m_grid.size(); i++)
	{
		for (int j = 0; j < m_grid[i].size(); j++)
		{
			if (m_grid[i][j]->isWall())
				m_grid[i][j]->setColor(Colour(0, 0, 255));
			else
			{
				if (i % 2 == 0)
				{
					if (j % 2 == 0)
						m_grid[i][j]->setColor(Colour(0, 0, 0));
					else
						m_grid[i][j]->setColor(Colour(255, 255, 255));
				}
				else
				{
					if (j % 2 != 0)
						m_grid[i][j]->setColor(Colour(0, 0, 0));
					else
						m_grid[i][j]->setColor(Colour(255, 255, 255));
				}
			}
			m_grid[i][j]->setMarked(false);
			m_grid[i][j]->setCostSoFar(0);
			m_grid[i][j]->setHeuristic(0);
			m_grid[i][j]->setFScore(0);
		}
	}
}