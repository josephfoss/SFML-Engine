#pragma once
#include "Tile.h"
#include "Entity.h"

class TileMap
{
private:
	//three dimensional vector.
	//x, y, z
	//x and y hold positions, z holds the layer.
	std::vector<std::vector<std::vector<Tile*>>> map;
	sf::Texture tileSheet;
	sf::Vector2u maxSize;
	sf::Vector2f maxSizeWorld;
	sf::RectangleShape collisionBox;

	void clear();
	void initMap(unsigned width, unsigned height, unsigned layers);

	std::string texture_file;
	unsigned layers;
	float gridSizeF;
	unsigned gridSizeU;

	//culling variables
	int fromX;
	int toX;
	int fromY;
	int toY;
	int layer;
public:
	TileMap(float gridSize, unsigned width, unsigned height, unsigned layers, std::string texture_file);
	~TileMap();

	sf::Texture* getTileSheet();

	unsigned getNumLayers();

	void addTile(const unsigned x, const unsigned y, const unsigned z, const sf::IntRect& rect, bool collision, short type);
	void removeTile(const unsigned x, const unsigned y, const unsigned z);

	void saveToFile(const std::string path);
	void loadFromFile(const std::string path);

	void updateCollision(Entity* entity, const float& dt);

	void update();
	void render(sf::RenderTarget& target, unsigned layer, Entity* entity = nullptr);
};

