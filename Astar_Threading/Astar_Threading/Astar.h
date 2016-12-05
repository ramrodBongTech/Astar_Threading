#include "stdafx.h"
#include <list>
#include <queue>
#include "Grid.h"

class NodeSearchCostComparer
{
public:
	bool operator() (Tile* n1, Tile* n2)
	{
		return n1->getCostSoFar() > n2->getCostSoFar();
	}
};

void process(Tile* t)
{
}

void Astar(Tile* pStart, Tile* pDest, Grid& grid, std::vector<Tile*>& path)
{
	bool destFound = false;
	int costSoFar = 9999999;

	if (pStart != 0) {
		for (int row = 0; row < grid.getNumTiles(); row++)
		{
			for (int col = 0; col < grid.getNumTiles(); row++)
			{
				Tile* _current = grid.getTile(std::pair<int, int>(col, row));
				if (_current != nullptr)
				{
					Point2D _distance = Point2D(_current->getPosition().x - pDest->getPosition().x, _current->getPosition().y - pDest->getPosition().y );
					_current->setHeuristic(std::sqrt((_distance.x * _distance.x) + (_distance.y * _distance.y)));

					_current->setCostSoFar(99999);
				}
			}
		}

		pStart->setCostSoFar(0);

		std::priority_queue<Tile*, std::vector<Tile*>, NodeSearchCostComparer> nodeQueue;

		// place the first node on the queue, and mark it.
		nodeQueue.push(pStart);
		pStart->setMarked(true);

		std::vector<Tile*> finished;

		// loop through the queue while there are nodes in it.
		while (nodeQueue.size() != 0)
		{
			Tile* currNode = nodeQueue.top();

			if (currNode != pDest && currNode != pStart)
			{
				currNode->setColor(Colour(125,125,125));
			}

			std::vector<Tile*> _neighbourNodes = grid.getNeighbourNodes(currNode->getIndex());

			for (int i = 0; i < _neighbourNodes.size(); i++)
			{
				if (_neighbourNodes[i] != pDest)
				{
					if (_neighbourNodes[i] != currNode->getPrevious() && std::find(finished.begin(), finished.end(), _neighbourNodes[i]) == finished.end())
					{
						float distC = (_neighbourNodes[i]->getWeight() + currNode->getCostSoFar()) + _neighbourNodes[i]->getHeuristic();
						if (distC < _neighbourNodes[i]->getCostSoFar())
						{
							_neighbourNodes[i]->setCostSoFar(_neighbourNodes[i]->getWeight() + currNode->getCostSoFar());
							_neighbourNodes[i]->setPrevious(currNode);

							if (!_neighbourNodes[i]->isMarked())
							{
								_neighbourNodes[i]->setMarked(true);
								nodeQueue.push(_neighbourNodes[i]);
							}
						}
					}
				}
				else
				{
					destFound = true;
					finished.push_back(currNode);
					process(currNode);

					float distC = (_neighbourNodes[i]->getWeight() + currNode->getCostSoFar()) + _neighbourNodes[i]->getHeuristic();
					if (distC < _neighbourNodes[i]->getCostSoFar())
					{
						_neighbourNodes[i]->setCostSoFar(_neighbourNodes[i]->getWeight() + currNode->getCostSoFar());
						_neighbourNodes[i]->setPrevious(currNode);
					}

					nodeQueue = std::priority_queue <Tile*, std::vector<Tile*>, NodeSearchCostComparer>();

					Tile* _temp = currNode;
					path.push_back(_temp);
					while (_temp->getPrevious() != 0)
					{
						path.push_back(_temp->getPrevious());
						_temp = _temp->getPrevious();
					}
					break;
				}
			}

			
			if (!destFound)
			{
				finished.push_back(currNode);
				process(currNode);
				nodeQueue.pop();
			}
		}
	}