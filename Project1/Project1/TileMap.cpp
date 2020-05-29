#include "stdafx.h"
#include "TileMap.h"


TileMap::TileMap(float gridSize, unsigned width, unsigned height, unsigned layers, std::string texture_file)
{
	gridSizeF = gridSize;
	gridSizeU = static_cast<unsigned>(gridSizeF);
	maxSize.x = width;
	maxSize.y = height;
	maxSizeWorld.x = static_cast<float>(width) * gridSize;
	maxSizeWorld.y = static_cast<float>(height) * gridSize;
	this->layers = layers;
	this->texture_file = texture_file;

	fromX = 0;
	toX = 0;
	fromY = 0;
	toY = 0;
	layer = 1;

	initMap(maxSize.x, maxSize.y, layers);

	tileSheet.loadFromFile(texture_file);

	collisionBox.setSize(sf::Vector2f(gridSize, gridSize));
	collisionBox.setFillColor(sf::Color(255, 0, 0, 50));
}


void TileMap::clear()
{
	for (size_t x = 0; x < maxSize.x; x++)
	{
		for (size_t y = 0; y < maxSize.y; y++)
		{
			for (size_t z = 0; z < layers; z++)
			{
				delete map[x][y][z];
			}
			map[x][y].clear();
		}
		map[x].clear();
	}

	//clear vector
	map.clear();
}

void TileMap::initMap(unsigned width, unsigned height, unsigned layers)
{
	//x = a vector of vector of tiles
	//y = a vector of tiles
	//z = a tile

	map.reserve(maxSize.x);
	for (size_t x = 0; x < maxSize.x; x++)
	{
		map.push_back(std::vector<std::vector<Tile*>>());

		for (size_t y = 0; y < maxSize.y; y++)
		{
			map[x].reserve(maxSize.y);
			map[x].push_back(std::vector<Tile*>());

			for (size_t z = 0; z < layers; z++)
			{
				map[x][y].reserve(layers);
				map[x][y].push_back(nullptr);
			}
		}
	}
}

TileMap::~TileMap()
{
	clear();
}

sf::Texture* TileMap::getTileSheet()
{
	return &tileSheet;
}

unsigned TileMap::getNumLayers()
{
	return layers;
}

void TileMap::addTile(const unsigned x, const unsigned y, const unsigned z, const sf::IntRect& rect, bool collision, short type)
{
	if (x < maxSize.x && x >= 0 && y < maxSize.y && y >= 0 && z < layers && z >= 0)
	{
		if (map[x][y][z] == nullptr)
		{
			map[x][y][z] = new Tile(x, y, gridSizeF, tileSheet, rect, collision, type);
		}
	}
}

void TileMap::removeTile(const unsigned x, const unsigned y, const unsigned z)
{
	if (x < maxSize.x && x >= 0 && y < maxSize.y && y >= 0 && z < layers && z >= 0)
	{
		if (map[x][y][z] != nullptr)
		{
			delete map[x][y][z];
			map[x][y][z] = nullptr;
		}
	}
}

void TileMap::saveToFile(const std::string path)
{
	/*
		Format:
		 - Size (x, y)
		 - Grid Size
		 - Layers
		 - Texture File Path
		 - Tile Grid Position (x, y)
		 - Tile TextureRect (x, y)
		 - Tile Type
	*/

	int counter = 0;
	std::ofstream fout;

	fout.open(path);

	if (fout.is_open())
	{
		std::cout << "Tile map saving...\n";

		fout << maxSize.x << " " << maxSize.y << "\n"
			 << gridSizeU << "\n"
			 << layers << "\n"
			 << texture_file << "\n";

		for (size_t x = 0; x < maxSize.x; x++)
		{
			for (size_t y = 0; y < maxSize.y; y++)
			{
				for (size_t z = 0; z < layers; z++)
				{
					if (map[x][y][z])
					{
						fout << x << " " << y << " " << z << " " << map[x][y][z]->getAsString() << " \n";
						counter++;
					}
				}
			}
		}
	}
	
	std::cout << "Finished save. (" << counter << ") Tiles saved.\n";

	fout.close();
}

void TileMap::loadFromFile(const std::string path)
{
	int counter = 0;
	std::ifstream fin;

	fin.open(path);

	if (fin.is_open())
	{
		std::cout << "File path open. Loading tiles.\n";

		unsigned x = 0, y = 0, z = 0;
		unsigned trX = 0, trY = 0;
		bool collision = false;
		short type = 0;

		//take in primary variables.
		fin >> maxSize.x >> maxSize.y >> gridSizeF >> layers >> texture_file;
		gridSizeU = static_cast<unsigned>(gridSizeF);

		//clear the grid
		clear();
		
		//resize the map
		initMap(maxSize.x, maxSize.y, layers);

		//load new texture file
		tileSheet.loadFromFile(texture_file);

		//load tiles
		while(fin >> x >> y >> z >> trX >> trY >> collision >> type)
		{
			counter++;
			map[x][y][z] = new Tile(x, y, gridSizeF, tileSheet, sf::IntRect(trX, trY, 120, 120), collision, type);
		}
	}

	std::cout << "Finished load. (" << counter << ") Tiles loaded.\n";

	fin.close();
}

void TileMap::updateCollision(Entity* entity, const float& dt)
{
	//disallows the player from moving outside of the bounds. 
	if (entity->getPosition().x < 0.f)
	{
		entity->setPosition(0.f, entity->getPosition().y);
		entity->getMoveComp().stopVelocityX();
	}
	else if (entity->getPosition().x + entity->getGlobalBounds().width > maxSizeWorld.x)
	{
		entity->setPosition(maxSizeWorld.x - entity->getGlobalBounds().width, entity->getPosition().y);
		entity->getMoveComp().stopVelocityX();
	}

	if (entity->getPosition().y < 0.f)
	{
		entity->setPosition(entity->getPosition().x, 0.f);
		entity->getMoveComp().stopVelocityY();
	}
	else if (entity->getPosition().y + entity->getGlobalBounds().height > maxSizeWorld.y)
	{
		entity->setPosition(entity->getPosition().x, maxSizeWorld.y - entity->getGlobalBounds().height);
		entity->getMoveComp().stopVelocityY();
	}

	this->fromX = entity->getGridPosition(this->gridSizeU).x - 2;
	if (this->fromX < 0)
		this->fromX = 0;
	else if (this->fromX > this->maxSizeWorld.x)
		this->fromX = this->maxSizeWorld.x;

	this->toX = entity->getGridPosition(this->gridSizeU).x + 4;
	if (this->toX < 0)
		this->toX = 0;
	else if (this->toX > this->maxSizeWorld.x)
		this->toX = this->maxSizeWorld.x;

	this->fromY = entity->getGridPosition(this->gridSizeU).y - 2;
	if (this->fromY < 0)
		this->fromY = 0;
	else if (this->fromY > this->maxSizeWorld.y)
		this->fromY = this->maxSizeWorld.y;

	this->toY = entity->getGridPosition(this->gridSizeU).y + 4;
	if (this->toY < 0)
		this->toY = 0;
	else if (this->toY > this->maxSizeWorld.y)
		this->toY = this->maxSizeWorld.y;


	for (int x = this->fromX; x < this->toX; x++)
	{
		for (int y = this->fromY; y < this->toY; y++)
		{
			if (map[x][y][1])
			{
				sf::FloatRect playerBounds = entity->getGlobalBounds();
				sf::FloatRect wallBounds = this->map[x][y][1]->getGlobalBounds();
				sf::FloatRect nextPositionBounds = entity->getNextPosition(dt);

				if (this->map[x][y][1]->getCollision() &&
					this->map[x][y][1]->intersects(nextPositionBounds)
					)
				{
					//Bottom collision
					if (playerBounds.top < wallBounds.top
						&& playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
						&& playerBounds.left < wallBounds.left + wallBounds.width
						&& playerBounds.left + playerBounds.width > wallBounds.left
						)
					{
						entity->stopVelocityY();
						entity->setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
					}

					//Top collision
					else if (playerBounds.top > wallBounds.top
						&& playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
						&& playerBounds.left < wallBounds.left + wallBounds.width
						&& playerBounds.left + playerBounds.width > wallBounds.left
						)
					{
						entity->stopVelocityY();
						entity->setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
					}

					//Right collision
					if (playerBounds.left < wallBounds.left
						&& playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
						&& playerBounds.top < wallBounds.top + wallBounds.height
						&& playerBounds.top + playerBounds.height > wallBounds.top
						)
					{
						entity->stopVelocityX();
						entity->setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
					}

					//Left collision
					else if (playerBounds.left > wallBounds.left
						&& playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
						&& playerBounds.top < wallBounds.top + wallBounds.height
						&& playerBounds.top + playerBounds.height > wallBounds.top
						)
					{
						entity->stopVelocityX();
						entity->setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
					}
				}
			}
		}
	}
}

void TileMap::update()
{

}

void TileMap::render(sf::RenderTarget& target, unsigned layer, Entity* entity)
{
	if (entity)
	{
		fromX = entity->getGridPosition(gridSizeU).x - 10;
		if (fromX < 0)
			fromX = 0;
		else if (fromX > maxSizeWorld.x)
			fromX = maxSizeWorld.x;

		toX = entity->getGridPosition(gridSizeU).x + 10;
		if (toX < 0)
			toX = 0;
		else if (toX > maxSizeWorld.x)
			toX = maxSizeWorld.x;

		fromY = entity->getGridPosition(gridSizeU).y - 10;
		if (fromY < 0)
			fromY = 0;
		else if (fromY > maxSizeWorld.y)
			fromY = maxSizeWorld.y;

		toY = entity->getGridPosition(gridSizeU).y + 10;
		if (toY < 0)
			toY = 0;
		else if (toY > maxSizeWorld.y)
			toY = maxSizeWorld.y;


		for (int x = fromX; x < toX; x++)
		{
			for (int y = fromY; y < toY; y++)
			{
				if (y < 50 && x < 50)
				{
					if (map[x][y][layer])
					{
						map[x][y][layer]->render(target);
						if (map[x][y][layer]->getCollision())
						{
							collisionBox.setPosition(map[x][y][layer]->getPosition());
							target.draw(collisionBox);
						}
					}
				}
			}
		}
	}
	else
	{
		for (size_t x = 0; x < maxSize.x; x++)
		{
			for (size_t y = 0; y < maxSize.y; y++)
			{
				if (map[x][y][layer])
				{
					map[x][y][layer]->render(target);
					if (map[x][y][layer]->getCollision())
					{
						collisionBox.setPosition(map[x][y][layer]->getPosition());
						target.draw(collisionBox);
					}
				}
			}
		}
	}
}
