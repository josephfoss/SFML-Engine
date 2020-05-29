#pragma once
//Abstract Class

enum TileType 
{
	DEFAULT = 0,
	WALL,
	ENTRANCE,
	EXIT,
	numOfElements // the value of this element will equal the amount of elements before it
};

class Tile
{
private:

protected:
	sf::RectangleShape shape;
	bool collision;
	short type;

public:
	Tile(unsigned x, unsigned y, float gridSize, sf::Texture& texture, const sf::IntRect& rect, bool collision = false, short type = TileType::DEFAULT);
	Tile();
	virtual ~Tile();

	const std::string getAsString() const;
	static std::string getTileType(int i);
	const sf::FloatRect getGlobalBounds() const;
	const bool& getCollision() const;
	const bool intersects(const sf::FloatRect bounds) const;
	const sf::Vector2f& getPosition() const;

	void update();
	void render(sf::RenderTarget& target);
};

