#include "stdafx.h"
#include "Grid.h"

Grid::Grid(float width, float height, int numTiles)
{
	float tileWidth = width / numTiles;
	float tileHeight = height / numTiles;

	for (int row = 0; row < numTiles; row++)
	{
		std::vector<Tile*> tiles;

		for (int col = 0; col < numTiles; col++)
		{
			// Fix if statement for exact board in spec
			if ((numTiles == 30 && ((col == 5 && (row >= 0 && row <= numTiles * 0.8)) ||
				(col == 15 && (row > 2 && row <= numTiles * 0.7)) ||
				(col == 25 && (row > 5 && row <= numTiles * 0.9))))
				||
				(numTiles == 100 && ((col == 15 && (row >= 0 && row <= numTiles * 0.8)) ||
				(col == 30 && (row > 7 && row <= numTiles * 0.95)) ||
				(col == 45 && (row > 3 && row <= numTiles * 0.91)) ||
				(col == 60 && (row < numTiles && row >= numTiles * 0.2)) ||
				(col == 75 && (row > 2 && row <= numTiles * 0.7)) ||
				(col == 90 && (row > 5 && row <= numTiles * 0.9))))
				||
				(numTiles == 1000 && (col == 50 && (row >= 0 && row <= numTiles * 0.8)) ||
				(col == 100 && (row > 7 && row <= numTiles * 0.95)) ||
				(col == 150 && (row > 3 && row <= numTiles * 0.91)) ||
				(col == 200 && (row < numTiles && row >= numTiles * 0.2)) ||
				(col == 250 && (row > 2 && row <= numTiles * 0.7)) ||
				(col == 300 && (row > 5 && row <= numTiles * 0.9)) ||
				(col == 350 && (row >= 0 && row <= numTiles * 0.8)) ||
				(col == 400 && (row > 7 && row <= numTiles * 0.95)) ||
				(col == 450 && (row > 3 && row <= numTiles * 0.91)) ||
				(col == 500 && (row < numTiles && row >= numTiles * 0.2)) ||
				(col == 550 && (row > 2 && row <= numTiles * 0.7)) ||
				(col == 600 && (row > 5 && row <= numTiles * 0.9)) ||
				(col == 650 && (row > 2 && row <= numTiles * 0.7)) ||
				(col == 700 && (row > 5 && row <= numTiles * 0.9)) ||
				(col == 750 && (row > 7 && row <= numTiles * 0.95)) ||
				(col == 800 && (row > 3 && row <= numTiles * 0.91)) ||
				(col == 850 && (row > 2 && row <= numTiles * 0.7)) ||
				(col == 900 && (row > 5 && row <= numTiles * 0.9))))
			{
				Tile* t = new Tile(tileWidth, tileHeight, Point2D(col * tileWidth, row * tileHeight), std::pair<int, int>(row, col));
				t->setWall();
				tiles.push_back(t);
			}
			else
			{
				tiles.push_back(new Tile(tileWidth, tileHeight, Point2D(col * tileWidth, row * tileHeight), std::pair<int, int>(row, col)));

				if (row % 2 == 0)
				{
					if (col % 2 == 0)
						tiles[col]->setColor(Colour(0, 0, 0));
					else
						tiles[col]->setColor(Colour(255, 255, 255));
				}
				else
				{
					if (col % 2 != 0)
						tiles[col]->setColor(Colour(0, 0, 0));
					else
						tiles[col]->setColor(Colour(255, 255, 255));
				}
			}
		}

		m_grid.push_back(tiles);
	}
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

//int Grid::getNumTiles(){ return m_grid.size(); }

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
		}
	}
}