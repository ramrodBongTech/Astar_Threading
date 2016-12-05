#include "stdafx.h"
#include <iostream>
#include <queue>
using namespace std;

#include "Game.h"

const int SCREEN_FPS = 100;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const float	SCREEN_WIDTH = 1200.0f;
const float	SCREEN_HEIGHT = 1200.0f;

/*template<typename T>
class NodeSearchCostComparer 
{
public:
	bool operator() (T n1, T n2)
	{
		return n1->getFScore() > n2->getFScore();
	}
};*/

class NodeSearchCostComparer { public: bool operator() (Tile* n1, Tile* n2) { return n1->getFScore() > n2->getFScore(); } };

/*template<typename T>
class custom_priority_queue : public std::priority_queue<T, std::vector<T>, NodeSearchCostComparer>
{
public:

	bool remove(const T& value) {
		auto it = std::find(this->c.begin(), this->c.end(), value);
		if (it != this->c.end()) {
			this->c.erase(it);
			std::make_heap(this->c.begin(), this->c.end(), this->comp);
			return true;
		}
		else {
			return false;
		}
	}
};*/

Game::Game():
	m_lastTime(0),
	m_quit(false),
	m_numTiles(30),
	m_grid(new Grid(SCREEN_WIDTH, SCREEN_HEIGHT, m_numTiles)),
	m_player(new Player(SCREEN_WIDTH / m_numTiles, SCREEN_HEIGHT / m_numTiles, Point2D(0, 0), std::pair<int, int>(0, 0)))
{}

Game::~Game()
{
	delete m_grid;
	m_grid = nullptr;
	delete m_player;
	m_player = nullptr;
	delete m_enemy;
	m_enemy = nullptr;
}

bool Game::init()
{
	cout << "Number Tiles: " << m_numTiles << endl;

	Tile* _t = m_grid->getTile(std::pair<int, int>(5,5/*rand() % m_numTiles, rand() % m_numTiles*/));

	while (_t->isWall())
		_t = m_grid->getTile(std::pair<int, int>(rand() % m_numTiles, rand() % m_numTiles));

	m_enemy = new Enemy(SCREEN_WIDTH / m_numTiles, SCREEN_HEIGHT / m_numTiles, _t->getPosition(), _t->getIndex());

	//creates our renderer, which looks after drawing and the window
	m_renderer.init(Size2D(SCREEN_WIDTH, SCREEN_HEIGHT), "A* Threading");

	m_inputManager.AddListener(EventListener::Event::ONE, this);
	m_inputManager.AddListener(EventListener::Event::TWO, this);
	m_inputManager.AddListener(EventListener::Event::THREE, this);
	m_inputManager.AddListener(EventListener::Event::UP, this);
	m_inputManager.AddListener(EventListener::Event::DOWN, this);
	m_inputManager.AddListener(EventListener::Event::LEFT, this);
	m_inputManager.AddListener(EventListener::Event::RIGHT, this);
	m_inputManager.AddListener(EventListener::Event::QUIT, this);
	m_inputManager.AddListener(EventListener::Event::R, this);
	m_inputManager.AddListener(EventListener::Event::RETURN, this);

	return true;
}

void Game::destroy()
{
	m_renderer.destroy();
}

//** calls update on all game entities*/
void Game::update()
{
	unsigned int currentTime = LTimer::gameTime();			//millis since game started
	float deltaTime = (currentTime - m_lastTime) / 1000.0;	//time since last update

	m_lastTime = currentTime;
}

//** calls render on all game entities*/
void Game::render()
{
	m_renderer.clear(Colour(0, 0, 0));

	m_grid->draw(m_renderer);
	m_player->draw(m_renderer);
	m_enemy->draw(m_renderer);

	m_renderer.present();
}

/** update and render game entities*/
void Game::loop()
{
	LTimer capTimer;

	int frameNum = 0;
	while (!m_quit)
	{
		capTimer.start();

		m_inputManager.ProcessInput();

		update();
		render();

		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME)
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
	}
}

void Game::onEvent(EventListener::Event evt)
{
	switch (evt)
	{
	case(EventListener::Event::ONE) :
		m_numTiles = 30;
		cout << "Number Tiles: " << m_numTiles << endl;
		break;
	case(EventListener::Event::TWO) :
		m_numTiles = 100;
		cout << "Number Tiles: " << m_numTiles << endl;
		break;
	case(EventListener::Event::THREE) :
		m_numTiles = 1000;
		cout << "Number Tiles: " << m_numTiles << endl;
		break;
	case(EventListener::Event::UP) :
		if (m_grid->getTile(std::pair<int, int>(m_player->getIndex().first - 1, m_player->getIndex().second)) != nullptr)
			m_player->moveUp();
		break;
	case(EventListener::Event::DOWN) :
		if (m_grid->getTile(std::pair<int, int>(m_player->getIndex().first + 1, m_player->getIndex().second)) != nullptr)
			m_player->moveDown();
		break;
	case(EventListener::Event::LEFT) :
		if (m_grid->getTile(std::pair<int, int>(m_player->getIndex().first, m_player->getIndex().second - 1)) != nullptr)
			m_player->moveLeft();
		break;
	case(EventListener::Event::RIGHT) :
		if (m_grid->getTile(std::pair<int, int>(m_player->getIndex().first, m_player->getIndex().second + 1)) != nullptr)
			m_player->moveRight();
		break;
	case(EventListener::Event::QUIT) :
		m_quit = true;
		break;
	case(EventListener::Event::R) :
		resetBoard();
		break;
	case(EventListener::Event::RETURN) :
		Astar();
		break;
	default:
		break;
	}
}

void Game::resetBoard()
{
	delete m_grid;
	m_grid = new Grid(SCREEN_WIDTH, SCREEN_HEIGHT, m_numTiles);
	
	delete m_player;
	m_player = new Player(SCREEN_WIDTH / m_numTiles, SCREEN_HEIGHT / m_numTiles, Point2D(0, 0), std::pair<int, int>(0, 0));
	
	delete m_enemy;
	Tile* _t = m_grid->getTile(std::pair<int, int>(rand() % m_numTiles, rand() % m_numTiles));

	while (_t == nullptr || _t->isWall())
		_t = m_grid->getTile(std::pair<int, int>(rand() % m_numTiles, rand() % m_numTiles));

	m_enemy = new Enemy(SCREEN_WIDTH / m_numTiles, SCREEN_HEIGHT / m_numTiles, _t->getPosition(), _t->getIndex());
}

void Game::Astar()
{
	m_grid->resetBoard();

	Tile* start = m_grid->getTile(m_player->getIndex());
	Tile* end = m_grid->getTile(m_enemy->getIndex());

	//custom_priority_queue<Tile*> open = custom_priority_queue<Tile*>();
	priority_queue<Tile*, std::vector<Tile*>, NodeSearchCostComparer> open = priority_queue<Tile*, std::vector<Tile*>, NodeSearchCostComparer>();
	std::vector<Tile*> closed = std::vector<Tile*>();

	start->setMarked(true);
	start->setCostSoFar(0);
	start->setHeuristic(m_grid->cost(start, end));
	start->setFScore(m_grid->cost(start, end));
	open.push(start);

	Tile* current = nullptr;

	while (!open.empty())
	{
		current = open.top();
		open.pop();
		closed.push_back(current);

		if (current == end)
			break;

		std::vector<Tile*> _neighbourNodes = m_grid->getNeighbourNodes(current->getIndex());
		for (Tile* next : _neighbourNodes)
		{
			if (std::find(closed.begin(), closed.end(), next) == closed.end())
			{
				float g = current->getCostSoFar() + m_grid->cost(current, next);
				float h = m_grid->cost(next, end);
				float f = g + h;

				if (!next->isMarked())
				{
					next->setCostSoFar(g);
					next->setHeuristic(h);
					next->setFScore(f);

					next->setMarked(true);
					next->setPrevious(current);
					next->setColor(Colour(128, 128, 128));
					open.push(next);
				}
				else
				{
					if (g <= next->getCostSoFar())
					{
						next->setCostSoFar(g);
						next->setFScore(f);
						next->setPrevious(current);
					}
				}
			}
		}
	}
	while (current->getPrevious() != nullptr)
	{
		// Gets caught here, each node is a parent of the other so it keeps jumping from one to the other in an endless loop
		current->setColor(Colour(128, 0, 128));
		current = current->getPrevious();
	}
	int check = 0;

	/*								ATTEMPT 3 & 4
	while (open.size() != 0)
	{
		current = open.top();
		cout << "Current Node (" << current->getIndex().first << ", " << current->getIndex().second << ")" << endl;
		//current->setColor(Colour(128, 0, 128));
		std::vector<Tile*> _neighbourNodes = m_grid->getNeighbourNodes(current->getIndex());

		if (current == end)
			break;

		cout << "Cost So Far: " << current->getCostSoFar() << endl;

		for (Tile* next : _neighbourNodes)
		{
			if (std::find(closed.begin(), closed.end(), next) == closed.end())
			{
				float g = current->getCostSoFar() + m_grid->cost(current, next);
				cout << "Cost To (" << next->getIndex().first << ", " << next->getIndex().second << "): " << m_grid->cost(current, next) << endl;
				cout << "CSF + Cost To: " << g << endl;
				float h = m_grid->cost(next, end);
				cout << "Heuristic: " << h << endl;
				float f = g + h;
				cout << "Estimated CSF: " << current->getCostSoFar() + m_grid->cost(current, end) << endl;
				cout << "Estimated Distance: " << f << "\n" << endl;

				next->setCostSoFar(g);
				next->setHeuristic(h);
				next->setFScore(f);
				
				if (!next->isMarked())
				{
					next->setMarked(true);
					next->setColor(Colour(128, 128, 128));
					open.push(next);
				}
				//else
				//{
					if (current->getCostSoFar() >= next->getCostSoFar())
					{
						next->setPrevious(current);
					}
					//if (next->getHeuristic() <= current->getHeuristic())
					//{
						//current->setCostSoFar(g);
						//next->setPrevious(current);
						//current->setNext(next);
					//}
				//}
			}
		}
		closed.push_back(current);
		open.remove(current);
	}
*/

/*									ATTEMPT 1 & 2
	m_grid->resetColour();
	bool destFound = false;

	Tile* pStart = m_grid->getTile(m_player->getIndex());
	Tile* pDest = m_grid->getTile(m_enemy->getIndex());

	if (pStart != 0) {
		for (int row = 0; row < m_numTiles; row++)
		{
			for (int col = 0; col < m_numTiles; col++)
			{
				Tile* _current = m_grid->getTile(std::pair<int, int>(col, row));
				if (_current != nullptr)
				{
					Point2D _distance = Point2D(_current->getPosition().x - pDest->getPosition().x, _current->getPosition().y - pDest->getPosition().y);
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
				currNode->setColor(Colour(125, 125, 125));

			std::vector<Tile*> _neighbourNodes = m_grid->getNeighbourNodes(currNode->getIndex());

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
					//process(currNode);

					float distC = (_neighbourNodes[i]->getWeight() + currNode->getCostSoFar()) + _neighbourNodes[i]->getHeuristic();
					if (distC < _neighbourNodes[i]->getCostSoFar())
					{
						_neighbourNodes[i]->setCostSoFar(_neighbourNodes[i]->getWeight() + currNode->getCostSoFar());
						_neighbourNodes[i]->setPrevious(currNode);
					}

					nodeQueue = std::priority_queue <Tile*, std::vector<Tile*>, NodeSearchCostComparer>();

					Tile* _temp = currNode;
					m_path.push_back(_temp);
					while (_temp->getPrevious() != 0)
					{
						m_path.push_back(_temp->getPrevious());
						_temp = _temp->getPrevious();
					}
					break;
				}
			}


			if (!destFound)
			{
				finished.push_back(currNode);
				//process(currNode);
				nodeQueue.pop();
			}
		}
	}


	for (int i = 0; i < m_path.size(); i++)
	{
		m_path[i]->setColor(Colour(128,0,128));
	}
	m_path.clear();*/
}
