#include "stdafx.h"
#include "State.h"


State::State(StateData* stateData)
{
	keyTime = 0.f;
	keyTimeMax = 2.f;

	initFont();
	state_data = stateData;
	window = stateData->window;
	keybinds = stateData->keybinds;
	states = stateData->states;
	gridSize = stateData->gridSize;

	quit = false;
	paused = false;
}

State::~State()
{
	std::cout << "  - State Base Destroyed\n";
}

const bool& State::getQuit() const
{
	return quit;
}

void State::endState()
{
	quit = true;
}

void State::pauseState()
{
	paused = true;
}

void State::unpauseState()
{
	paused = false;
}

bool State::getKeyTime()
{
	if (keyTime >= keyTimeMax)
	{
		keyTime = 0.f;
		return true;
	}
	else
	{
		return false;
	}
}

void State::updateKeyTime(const float& dt)
{
	if (keyTime < keyTimeMax)
	{
		keyTime += 10.f * dt;
	}
}

void State::updateMousePositions(sf::View* view)
{
	mousePosScreen = sf::Mouse::getPosition();
	mousePosWindow = sf::Mouse::getPosition(*window);

	if(view)
		window->setView(*view);

	//gets the mouse position relative to the size of the window.
	//ex. (top left = (0,0), bottom right = (windowsize.x, windowsize.y))
	mousePosView = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	mousePosGrid = sf::Vector2u(static_cast<unsigned>(mousePosView.x) / static_cast<unsigned>(gridSize), static_cast<unsigned>(mousePosView.y) / static_cast<unsigned>(gridSize));

	window->setView(window->getDefaultView());
}

void State::initFont()
{
	if (!fontMain.loadFromFile("Assets/Fonts/AncientModernTales-a7Po.ttf"))
	{
		throw("Could not load font.");
	}
	if (!fontSecondary.loadFromFile("Assets/Fonts/AGoblinAppears-o2aV.ttf"))
	{
		throw("Could not load font.");
	}
}
