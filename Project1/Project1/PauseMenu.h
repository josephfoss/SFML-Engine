#pragma once
#include "Gui.h"

class PauseMenu
{
private:
	sf::Font& font;
	sf::Text menutext;

	//initial positions.
	sf::Vector2f containerPos;
	sf::Vector2f backgroundPos;
	sf::Vector2f textPos;
	sf::Vector2f exitButtonPos;

	sf::RectangleShape background;
	sf::RectangleShape container;

	std::map<std::string, gui::Button*> buttons;
	std::map<std::string, sf::Texture>& textures;

public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font, std::map<std::string, sf::Texture>& textures);
	virtual ~PauseMenu();

	std::map<std::string, gui::Button*>& getButtons();

	void initButtons();
	void initContainers(sf::RenderWindow& window);
	void initText();

	void update(sf::Vector2i& mousePosWindow);
	void render(sf::RenderTarget& target);
};

