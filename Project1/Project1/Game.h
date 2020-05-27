#pragma once

#include "MainMenuState.h"

class Game
{
private:
	sf::RenderWindow* window;
	sf::Event event;

	//window settings
	sf::VideoMode windowBounds = sf::VideoMode::getDesktopMode();
	std::string title = "";
	sf::ContextSettings windowSettings;
	unsigned frameRate = 120;
	bool verticalSync = false;
	unsigned antialiasing_level = 0;

	sf::Clock clock;
	float deltaTime;

	std::stack<State*> states;
	StateData stateData;
	float gridSize;

	std::map<std::string, sf::Keyboard::Key> keyMappings;

	void initializeWindow();
	void initializeKeybinds();
	void initializeStateData();
	void initializeStates();

public:
	Game();
	virtual ~Game();

	float lastTime = 0;

	//game loop functions
	void updateDeltaTime();
	void updateEvents();
	void update();
	void render();
	void run();
};


