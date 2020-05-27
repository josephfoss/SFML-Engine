#include "stdafx.h"
#include "Tile.h"

Tile::Tile(unsigned x, unsigned y, float gridSize, sf::Texture& texture, const sf::IntRect& rect, bool collision, short type)
{
	shape.setSize(sf::Vector2f(gridSize, gridSize));
	shape.setPosition(x * gridSize, y * gridSize);
	shape.setTexture(&texture);
	shape.setTextureRect(rect);

	this->collision = collision;
	this->type = type;
}

Tile::Tile()
{
	collision = false;
	type = 0;
}


Tile::~Tile()
{
}

const std::string Tile::getAsString() const
{
	std::stringstream ss;

	ss << shape.getTextureRect().left << " " << shape.getTextureRect().top << " " << collision << " " << type;

	return ss.str();
}

std::string Tile::getTileType(int i)
{
	std::string tileTypes[4] = { "DEFAULT", "WALL", "ENTRANCE", "EXIT" };

	return tileTypes[i];
}

const bool& Tile::getCollision() const
{
	return collision;
}

const sf::Vector2f& Tile::getPosition() const
{
	return shape.getPosition();
}

void Tile::update()
{

}

void Tile::render(sf::RenderTarget& target)
{
	target.draw(shape);
}
