#pragma once

#include "Player.h"

class StateData;

class State
{
protected:
	//pointer to a stack of state pointers...
	std::stack<State*>* states;
	StateData* state_data;
	sf::RenderWindow* window;
	sf::Font fontMain;
	sf::Font fontSecondary;

	std::map<std::string, sf::Keyboard::Key>* keybinds;
	std::map<std::string, sf::Keyboard::Key> stateKeybinds;

	bool quit;
	bool paused;

	float keyTime;
	float keyTimeMax;
	float gridSize;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Vector2u mousePosGrid;

	//Texture map
	std::map<std::string, sf::Texture> textures;

public:
	State(StateData* stateData);
	virtual ~State();

	const bool& getQuit() const;
	void endState();
	void pauseState();
	void unpauseState();
	bool getKeyTime();
	void updateKeyTime(const float& dt);

	virtual void updateMousePositions(sf::View* view = NULL);
	virtual void initFont();

	//pure virtual functions - these functions must be within child classes.
	virtual void initKeybinds() = 0;
	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;

};

class StateData
{
public:
	StateData() {};

	float gridSize;
	sf::RenderWindow* window;
	std::map<std::string, sf::Keyboard::Key>* keybinds;
	std::stack<State*>* states;

};

