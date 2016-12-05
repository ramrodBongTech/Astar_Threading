#include "stdafx.h"
#include "Player.h"

Player::Player(float width, float height, Point2D pos, std::pair<int, int> index) :
	m_rect(Rect(pos, Size2D(width, height))),
	m_colour(Colour(0, 255, 0)),
	m_index(index)
{}

Player::~Player(){}

void Player::draw(Renderer& renderer){ renderer.drawRect(m_rect, m_colour); }

std::pair<int, int> Player::getIndex(){ return m_index; }

void Player::setPosition(Point2D pos){ m_rect.pos = pos; }

void Player::moveUp()
{
	m_rect.pos.y -= m_rect.size.h;
	m_index.first--;
}

void Player::moveDown()
{
	m_rect.pos.y += m_rect.size.h;
	m_index.first++;
}

void Player::moveLeft()
{
	m_rect.pos.x -= m_rect.size.w;
	m_index.second--;
}

void Player::moveRight()
{
	m_rect.pos.x += m_rect.size.w;
	m_index.second++;
}
