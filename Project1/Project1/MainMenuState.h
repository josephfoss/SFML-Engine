#pragma once

#include "GameState.h"
#include "EditorState.h"
#include "Gui.h"

class MainMenuState : public State
{
private:
	sf::RectangleShape logo;
	sf::RectangleShape background;
	sf::Music ambiance;

	std::map<std::string, sf::Texture*> textures;
	std::map<std::string, gui::Button*> buttons;

	void initVariables();
	void initTextures();
	void initKeybinds();
	void initButtons();
	void initAudio();
public:
	MainMenuState(StateData* stateData);
	virtual ~MainMenuState();

	void updateButtons();
	void updateAudio();
	void updateInput(const float& dt);
	void update(const float& dt);
	void renderButtons(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);
};

