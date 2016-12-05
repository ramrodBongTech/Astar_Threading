#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(){}

Enemy::Enemy(float width, float height, Point2D pos, std::pair<int, int> index) :
	m_rect(pos, Size2D(width, height)),
	m_colour(Colour(255, 0, 0)),
	m_index(index)
{}

Enemy::~Enemy(){}

void Enemy::draw(Renderer& renderer){ renderer.drawRect(m_rect, m_colour); }

std::pair<int, int> Enemy::getIndex(){ return m_index; }

void Enemy::setPosition(Point2D pos){ m_rect.pos = pos; }