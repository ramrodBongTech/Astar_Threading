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
};

template<typename T>
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

class CostComparer { public: bool operator() (Tile* n1, Tile* n2) { return n1->getFScore() > n2->getFScore(); } };

struct ThreadData{
	Tile* start = nullptr;
	Tile* end = nullptr;
	Grid* grid;
};

int getCost(Tile* t1, Tile* t2)
{
	int n = std::sqrt(((t1->getPosition().x - t2->getPosition().x) * (t1->getPosition().x - t2->getPosition().x)) + ((t1->getPosition().y - t2->getPosition().y) * (t1->getPosition().y - t2->getPosition().y)));
	return n;
}

int threadedAstar(void* data)
{
	priority_queue<Tile*, std::vector<Tile*>, CostComparer> open = priority_queue<Tile*, std::vector<Tile*>, CostComparer>();
	std::vector<Tile*> closed = std::vector<Tile*>();
	std::map<Tile*, bool> marked = std::map<Tile*, bool>();
	std::map<Tile*, float> cost = std::map<Tile*, float>();
	std::map<Tile*, Tile*> previous = std::map<Tile*, Tile*>();

	ThreadData* tdata = (ThreadData*)data;
	Tile* start = tdata->start;
	Tile* end = tdata->end;
	Grid* grid = tdata->grid;

	marked[start] = true;
	cost[start] = 0;
	previous[start] = nullptr;
	start->setFScore(getCost(start, end));
	open.push(start);

	Tile* current = nullptr;

	while (!open.empty())
	{
		current = open.top();
		closed.push_back(current);
		open.pop();

		if (current == end)
			break;

		std::vector<Tile*> _neighbourNodes = grid->getNeighbourNodes(current->getIndex());
		for (Tile* next : _neighbourNodes)
		{
			if (std::find(closed.begin(), closed.end(), next) == closed.end())
			{
				int g = cost[current] + std::sqrt(((current->getPosition().x - next->getPosition().x) * (current->getPosition().x - next->getPosition().x)) + ((current->getPosition().y - next->getPosition().y) * (current->getPosition().y - next->getPosition().y)));
				int f = g + std::sqrt(((next->getPosition().x - end->getPosition().x) * (next->getPosition().x - end->getPosition().x)) + ((next->getPosition().y - end->getPosition().y) * (next->getPosition().y - end->getPosition().y)));

				if (!marked[next])
				{
					cost[next] = g;
					next->setFScore(f);

					marked[next] = true;
					previous[next] = current;
					//next->setColor(Colour(128, 128, 128));
					open.push(next);
				}
				else
				{
					if (g <= cost[next])
					{
						cost[next] = g;
						next->setFScore(f);
						previous[next] = current;
					}
				}
			}
		}
	}
	while (previous[current] != nullptr)
	{
		current->setColor(Colour(128, 0, 128));
		current = previous[current];
	}
	return 0;
}

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

	/*while (_t->isWall())
		_t = m_grid->getTile(std::pair<int, int>(rand() % m_numTiles, rand() % m_numTiles));*/

	m_enemy = new Enemy(SCREEN_WIDTH / m_numTiles, SCREEN_HEIGHT / m_numTiles, _t->getPosition(), _t->getIndex());

	_t = m_grid->getTile(std::pair<int, int>(5, 0));

	en2 = new Enemy(SCREEN_WIDTH / m_numTiles, SCREEN_HEIGHT / m_numTiles, _t->getPosition(), _t->getIndex());

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
	en2->draw(m_renderer);

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
	ThreadData td;

	switch (evt)
	{
	case(EventListener::Event::ONE) :
		m_numTiles = 30;
		resetBoard();
		cout << "Number Tiles: " << m_numTiles << endl;
		break;
	case(EventListener::Event::TWO) :
		m_numTiles = 100;
		resetBoard();
		cout << "Number Tiles: " << m_numTiles << endl;
		break;
	case(EventListener::Event::THREE) :
		m_numTiles = 1000;
		resetBoard();
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
		m_grid->resetBoard();
		
		td.start = m_grid->getTile(m_player->getIndex());
		td.end = m_grid->getTile(m_enemy->getIndex());
		td.grid = m_grid;

		m_thread1 = SDL_CreateThread(threadedAstar, "astar1", &td);
		//SDL_Delay(250);
		SDL_WaitThread(m_thread1, NULL);

		td.start = m_grid->getTile(m_player->getIndex());
		td.end = m_grid->getTile(en2->getIndex());
		td.grid = m_grid;

		m_thread2 = SDL_CreateThread(threadedAstar, "astar2", &td);
		//SDL_Delay(250);
		SDL_WaitThread(m_thread2, NULL);


		//Astar(m_grid->getTile(m_player->getIndex()), m_grid->getTile(m_enemy->getIndex()));
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

void Game::Astar(Tile* start, Tile* end)
{
	priority_queue<Tile*, std::vector<Tile*>, CostComparer> open = priority_queue<Tile*, std::vector<Tile*>, CostComparer>();
	std::vector<Tile*> closed = std::vector<Tile*>();
	std::map<Tile*, bool> marked = std::map<Tile*, bool>();
	std::map<Tile*, float> cost = std::map<Tile*, float>();
	std::map<Tile*, Tile*> previous = std::map<Tile*, Tile*>();

	int weight = 1;
	marked[start] = true;
	cost[start] = 0;
	previous[start] = nullptr;
	start->setFScore(getCost(start, end));
	open.push(start);

	Tile* current = nullptr;

	while (!open.empty())
	{
		current = open.top();
		closed.push_back(current);
		open.pop();

		if (current == end)
			break;

		std::vector<Tile*> _neighbourNodes = m_grid->getNeighbourNodes(current->getIndex());
		for (Tile* next : _neighbourNodes)
		{
			if (std::find(closed.begin(), closed.end(), next) == closed.end())
			{
				int g = cost[current] + getCost(current, next);
				int f = g + getCost(next, end);

				if (!marked[next])
				{
					cost[next] = g;
					next->setFScore(f);

					marked[next] = true;
					previous[next] = current;
					//next->setColor(Colour(128, 128, 128));
					open.push(next);
				}
				else
				{
					if (g <= cost[next])
					{
						cost[next] = g;
						next->setFScore(f);
						previous[next] = current;
					}
				}
			}
		}
	}
	while (previous[current] != nullptr)
	{
		current->setColor(Colour(128, 0, 128));
		current = previous[current];
	}
	int check = 0;
}

int Game::getCost(Tile* t1, Tile* t2)
{
	//int c = abs((t2->getPosition().x - t1->getPosition().x)) + std::abs((t2->getPosition().y - t1->getPosition().y));
	//int b = (abs(t2->getIndex().first - t1->getIndex().first) + abs(t2->getIndex().second - t1->getIndex().second));
	int n = std::sqrt(((t1->getPosition().x - t2->getPosition().x) * (t1->getPosition().x - t2->getPosition().x)) + ((t1->getPosition().y - t2->getPosition().y) * (t1->getPosition().y - t2->getPosition().y)));
	return n;
}

