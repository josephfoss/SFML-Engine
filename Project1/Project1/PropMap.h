#pragma once
#include "Prop.h"

class PropMap
{
private:
	std::vector<Prop*> map;
	sf::Texture propSheet;
	sf::Vector2u maxSize;
	
	std::string texture_file;

	void clear();
public:
	PropMap(unsigned width, unsigned height, std::string texture_file);
	~PropMap();

	sf::Texture* getPropSheet();
	
	std::vector<Prop*> getMap();

	void addProp(const unsigned x, const unsigned y, const sf::IntRect& rect, bool clickable, int id, sf::Font* font, bool breakable);
	void removeProp(const unsigned x, const unsigned y);

	void saveToFile(const std::string path);
	void loadFromFile(const std::string path, sf::Font* font);

	void update(const sf::Vector2f& mousePosView);
	void render(sf::RenderTarget& target, bool editorMode);
};

