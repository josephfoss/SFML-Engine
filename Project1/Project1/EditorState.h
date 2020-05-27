#pragma once
#include "State.h"
#include "Gui.h"
#include "PauseMenu.h"
#include "TileMap.h"
#include "PropMap.h"

enum EditorMode{TILES = 0, PROPS};

class EditorState :
	public State
{
private:
	sf::View camera;
	sf::Event event;
	PauseMenu* pausemenu;
	sf::Text cursorText;
	TileMap* tileMap;
	PropMap* propMap;

	gui::DropDownMenu* ddmenu;
	std::map<std::string, gui::Button*> buttons;

	sf::IntRect textureRect;
	sf::IntRect propTextureRect;

	sf::RectangleShape selectorRect;

	gui::TextureSelector* textureSelector;
	gui::TextureSelector* propSelector;

	EditorMode editorMode = EditorMode::TILES;
	float currentLayer;
	float zoomlevel;
	float cameraSpeed;

	int id;
	bool breakable;
	bool clickable;
	bool collision;
	short type;

	void initCamera();
	void initVariables();
	void initTextures();
	void initKeybinds();
	void initText();
	void initGui();
	void initTileMap();
public:
	EditorState(StateData* stateData);
	virtual ~EditorState();

	std::string getPropModeFromId(int id);

	void updateButtons();
	void updatePauseMenuButtons(const float& dt);
	void updateInput(const float& dt);
	void updateEditorInput(const float& dt);
	void updateGui(const float& dt);
	void update(const float& dt);

	void renderGui(sf::RenderTarget& target);
	void render(sf::RenderTarget* target = nullptr);
};

