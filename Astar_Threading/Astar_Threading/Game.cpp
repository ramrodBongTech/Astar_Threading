#include "stdafx.h"
#include <iostream>
#include <queue>
using namespace std;

#include "Game.h"

const int SCREEN_FPS = 100;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const float	SCREEN_WIDTH = 1000.0f;
const float	SCREEN_HEIGHT = 1000.0f;

class CostComparer { public: bool operator() (std::pair<Tile*, int> n1, std::pair<Tile*, int> n2) { return n1.second > n2.second; } };

int getCost(Tile* t1, Tile* t2)
{
	return std::sqrt(((t1->getPosition().x - t2->getPosition().x) * (t1->getPosition().x - t2->getPosition().x)) + ((t1->getPosition().y - t2->getPosition().y) * (t1->getPosition().y - t2->getPosition().y)));
}

void* Astar(Tile* start, Tile* end, Grid* grid)
{
	priority_queue<std::pair<Tile*, int>, std::vector<std::pair<Tile*, int>>, CostComparer> open = priority_queue<std::pair<Tile*, int>, std::vector<std::pair<Tile*, int>>, CostComparer>();
	std::vector<Tile*> closed = std::vector<Tile*>();
	std::map<Tile*, bool> marked = std::map<Tile*, bool>();
	std::map<Tile*, int> cost = std::map<Tile*, int>();
	std::map<Tile*, Tile*> previous = std::map<Tile*, Tile*>();

	marked[start] = true;
	cost[start] = 0;
	previous[start] = nullptr;
	open.push(std::pair<Tile*, int>(start, getCost(start, end)));

	Tile* current = nullptr;

	while (!open.empty())
	{
		current = open.top().first;
		closed.push_back(current);
		open.pop();

		if (current == end)
			break;

		std::vector<Tile*> _neighbourNodes = grid->getNeighbourNodes(current->getIndex());
		for (Tile* next : _neighbourNodes)
		{
			if (std::find(closed.begin(), closed.end(), next) == closed.end())
			{
				int g = cost[current] + getCost(current, next);
				int f = g + getCost(next, end);

				if (!marked[next])
				{
					cost[next] = g;
					marked[next] = true;
					previous[next] = current;
					open.push(std::pair<Tile*, int>(next, f));
				}
				else
				{
					if (g <= cost[next])
					{
						cost[next] = g;
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
	m_numEnemies(5),
	m_grid(new Grid(SCREEN_WIDTH, SCREEN_HEIGHT, m_numTiles)),
	m_player(new Player(SCREEN_WIDTH / m_numTiles, SCREEN_HEIGHT / m_numTiles, Point2D(0, 0), std::pair<int, int>(0, 0)))
{}

Game::~Game()
{
	delete m_grid;
	m_grid = nullptr;
	delete m_player;
	m_player = nullptr;
	m_enemies.clear();
}

bool Game::init()
{
	cout << "Number Tiles: " << m_numTiles << endl;

	for (int i = 0; i < m_numEnemies; i++)
	{
		std::pair<int, int> index = std::pair<int, int>(rand() % (m_numTiles - 1), rand() % (m_numTiles - 1));
		Tile* _t = m_grid->getTile(index);

		while (_t == nullptr)
		{
			index = std::pair<int, int>(rand() % (m_numTiles - 1), rand() % (m_numTiles - 1));
			_t = m_grid->getTile(index);
		}

		m_enemies.push_back(new Enemy(SCREEN_WIDTH / m_numTiles, SCREEN_HEIGHT / m_numTiles, _t->getPosition(), _t->getIndex()));
	}

	m_pool = new ThreadPool();

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
	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->draw(m_renderer);
	}

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
		m_numEnemies = 5;
		resetBoard();
		cout << "Number Tiles: " << m_numTiles << endl;
		break;
	case(EventListener::Event::TWO) :
		m_numTiles = 100;
		m_numEnemies = 50;
		resetBoard();
		cout << "Number Tiles: " << m_numTiles << endl;
		break;
	case(EventListener::Event::THREE) :
		m_numTiles = 1000;
		m_numEnemies = 500;
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
		
		for (int i = 0; i < m_enemies.size(); i++)
		{
			m_pool->addTask(std::bind(Astar, m_grid->getTile(m_player->getIndex()), m_grid->getTile(m_enemies[i]->getIndex()), m_grid));
		}
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
	
	m_enemies.clear();
	for (int i = 0; i < m_numEnemies; i++)
	{
		std::pair<int, int> index = std::pair<int, int>(rand() % (m_numTiles - 1), rand() % (m_numTiles - 1));
		Tile* _t = m_grid->getTile(index);

		while (_t == nullptr)
		{
			index = std::pair<int, int>(rand() % (m_numTiles - 1), rand() % (m_numTiles - 1));
			_t = m_grid->getTile(index);
		}

		m_enemies.push_back(new Enemy(SCREEN_WIDTH / m_numTiles, SCREEN_HEIGHT / m_numTiles, _t->getPosition(), _t->getIndex()));
	}
}
