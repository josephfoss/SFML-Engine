#include "stdafx.h"
#include "PropMap.h"

PropMap::PropMap(unsigned width, unsigned height, std::string texture_file)
{
	maxSize.x = width;
	maxSize.y = height;
	this->texture_file = texture_file;

	propSheet.loadFromFile(texture_file);
}

PropMap::~PropMap()
{
	clear();
}

sf::Texture* PropMap::getPropSheet()
{
	return &propSheet;
}

std::vector<Prop*> PropMap::getMap()
{
	return map;
}

void PropMap::clear()
{
	for (auto& i : map)
	{
		delete i;
	}

	//clear vector
	map.clear();
}

void PropMap::addProp(const unsigned x, const unsigned y, const sf::IntRect& rect, bool clickable, int id, sf::Font* font, bool breakable)
{
	if (x < maxSize.x && x >= 0 && y < maxSize.y && y >= 0)
	{
		map.push_back(new Prop(x, y, 120, propSheet, rect, clickable, id, font, breakable));
	}
}

void PropMap::removeProp(const unsigned x, const unsigned y)
{
	if (x < maxSize.x && x >= 0 && y < maxSize.y && y >= 0)
	{
		for (auto& i : map)
		{
			if (i)
			{
				if (sqrt(pow((i->getPosition().x - x), 2) + pow((i->getPosition().y - y), 2)) < 75)
				{
					ptrdiff_t pos = std::find(map.begin(), map.end(), i) - map.begin();
					if (map[pos])
					{
						map[pos] = nullptr;
					}
					delete i;
				}
			}
		}
	}
}

void PropMap::saveToFile(const std::string path)
{
	/*
		Format:
		 - Size (x, y)
		 - Layers
		 - Texture File Path
		 - Prop Position (x, y)
		 - Tile TextureRect (x, y)
		 - ID
		 - Clickable
		 - Breakable
	*/

	int counter = 0;
	std::ofstream fout;

	fout.open(path);

	if (fout.is_open())
	{
		std::cout << "Prop map saving...\n";

		fout << maxSize.x << " " << maxSize.y << "\n"
			<< texture_file << "\n";

		for (auto& i : map)
		{
			if (i)
			{
				fout << i->getPosition().x << " " << i->getPosition().y << " " << i->getAsString() << " \n";
				counter++;
			}
		}
	}

	std::cout << "Finished save. (" << counter << ") Props saved.\n";

	fout.close();
}

void PropMap::loadFromFile(const std::string path, sf::Font* font)
{
	int counter = 0;
	std::ifstream fin;

	fin.open(path);

	if (fin.is_open())
	{
		std::cout << "File path open. Loading Props.\n";

		unsigned x = 0, y = 0;
		unsigned trX = 0, trY = 0;
		int id = 0;
		bool clickable = false;
		bool breakable = false;

		//take in primary variables.
		fin >> maxSize.x >> maxSize.y >> texture_file;

		//clear the grid
		clear();

		//load new texture file
		propSheet.loadFromFile(texture_file);

		//load tiles
		while (fin >> x >> y >> trX >> trY >> clickable >> id >> breakable)
		{
			counter++;
			map.push_back(new Prop(x, y, 120, propSheet, sf::IntRect(trX, trY, 120, 120), clickable, id, font, breakable));
		}
	}

	std::cout << "Finished load. (" << counter << ") Props loaded.\n";

	fin.close();
}

void PropMap::update(const sf::Vector2f& mousePosView)
{
	for (auto& i : map)
	{
		if (i)
		{
			i->update(mousePosView);
		}
	}
}

void PropMap::render(sf::RenderTarget& target, bool editorMode)
{
	if (editorMode)
	{
		for (auto& i : map)
		{
			if (i)
			{
				i->render(target);
			}
		}
	}
	else
	{
		for (auto& i : map)
		{
			if (i)
			{
				if (i->getID() == 0)
				{
					i->render(target);
				}
			}
		}
	}

}
