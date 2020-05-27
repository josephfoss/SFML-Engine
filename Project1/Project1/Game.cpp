#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	gridSize = 120.f;

	initializeWindow();
	initializeKeybinds();
	initializeStateData();
	initializeStates();
}

Game::~Game()
{
	delete window;

	while (!states.empty())
	{
		delete states.top();
		states.pop();
	}
}


void Game::initializeWindow()
{
	std::ifstream fin("Properties/window.ini");

	if (fin.is_open())
	{
		std::getline(fin, title);
		fin >> windowBounds.width >> windowBounds.height;
		fin >> frameRate;
		fin >> verticalSync;
		fin >> antialiasing_level;
	}

	fin.close();

	windowSettings.antialiasingLevel = antialiasing_level;
	window = new sf::RenderWindow(windowBounds, title, sf::Style::Default, windowSettings);
	window->setFramerateLimit(frameRate);
	window->setVerticalSyncEnabled(verticalSync);

}

void Game::initializeStates()
{
	states.push(new MainMenuState(&stateData));
}

void Game::initializeKeybinds()
{
	std::ifstream fin("Properties/keybindings.ini");
	std::string key = "";
	int keyVal;


	while (!fin.eof())
	{
		fin >> key >> keyVal;
		//reading supported keybinds from a text file into the keybind map.
		//The map takes a sf::Keyboard::Key value but since were taking in an int,
		//the int must be casted to sf::Keyboard::Key.
		keyMappings[key] = (sf::Keyboard::Key)keyVal;
	}

	fin.close();
}

void Game::initializeStateData()
{
	stateData.window = window;
	stateData.keybinds = &keyMappings;
	stateData.states = &states;
	stateData.gridSize = gridSize;
}

void Game::updateDeltaTime()
{
	deltaTime = clock.restart().asSeconds();
}

void Game::updateEvents()
{
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();
	}
}

void Game::update()
{
	updateEvents();

	if (!states.empty())
	{
		//calls the update function of the top game state in the stack.
		states.top()->update(deltaTime);

		//if the game state returns a quit, the game state gets deleted from the std::stack.
		if (states.top()->getQuit())
		{
			states.top()->endState();
			delete states.top();
			states.pop();
		}
	}
	else
	{
		window->close();
	}
}

void Game::render()
{
	window->clear();

	if (!states.empty())
	{
		//calls the render function of the top game state in the stack.
		states.top()->render(window);
	}

	window->display();
}

void Game::run()
{
	while(window->isOpen())
	{
		updateDeltaTime();
		update();
		render();
	}
}