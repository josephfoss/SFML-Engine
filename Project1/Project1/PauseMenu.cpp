#include "stdafx.h"
#include "PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow& window, sf::Font& font, std::map<std::string, sf::Texture>& textures)
	: font(font), textures(textures)
{
	initContainers(window);
	initText();
	initButtons();
}

PauseMenu::~PauseMenu()
{
	auto i = buttons.begin();
	for (i = buttons.begin(); i != buttons.end(); i++)
	{
		delete i->second;
	}
}

std::map<std::string, gui::Button*>& PauseMenu::getButtons()
{
	return buttons;
}

void PauseMenu::initButtons()
{
	buttons["EXIT"] =
		new gui::Button(
			//position              dimensions
			sf::Vector2f(575, 200), sf::Vector2f(350, 170),
			//font     text    size fontcolor
			&font, "Exit", 75,
			//fontidle               fonthover                fontactive
			sf::Color(194, 10, 19), sf::Color(230, 10, 19), sf::Color(194, 10, 19),
			//idlecolor               hovercolor                activecolor
			sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0)
		);
}

void PauseMenu::initContainers(sf::RenderWindow& window)
{
	background.setSize(sf::Vector2f(1541, 937));
	background.setPosition(-20, -5);
	background.setFillColor(sf::Color(20, 20, 20, 100));

	container.setSize(sf::Vector2f(static_cast<float>(1500) / 2.f, static_cast<float>(window.getSize().y)));
	container.setFillColor(sf::Color(255, 255, 255, 255));
	container.setTexture(&textures["BUTTON"]);
	container.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x) / 2.f - container.getSize().x / 2.f, 30.f));
}

void PauseMenu::initText()
{
	menutext.setFont(font);
	menutext.setFillColor(sf::Color(204, 0, 17));
	menutext.setCharacterSize(70);
	menutext.setString("PAUSED");

	menutext.setPosition(container.getPosition().x + container.getSize().x / 2.f - menutext.getGlobalBounds().width / 2.f, container.getPosition().y + 40.f);
}

void PauseMenu::update(sf::Vector2i& mousePosWindow)
{
	for (auto& i : buttons)
	{
		i.second->update(mousePosWindow);
	}
}

void PauseMenu::render(sf::RenderTarget& target)
{
	target.draw(background);
	target.draw(container);

	for (auto& i : this->buttons)
	{
		i.second->render(target);
	}

	target.draw(menutext);
}
