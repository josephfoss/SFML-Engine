#pragma once

#include "State.h"
#include "PauseMenu.h"
#include "TileMap.h"
#include "PropMap.h"

class GameState : public State
{
private:
	Player* player;
	TileMap* tileMap;
	PropMap* propMap;
	PauseMenu* pausemenu;
	gui::ShopMenu* shop;
	gui::HUD* hud;
	sf::View camera;
	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	sf::Vector2f playerSpawn;

	void initDeferredRender();
	void initPlayers();
	void initKeybinds();
	void initTextures();
	void initGUI();
	void initTileMap();
public:
	GameState(StateData* stateData);
	virtual ~GameState();

	void loadNewMap(std::string filepath);

	void updateView();
	void updateButtons(const sf::Vector2f& mousePosView);
	void updatePauseMenuButtons();
	void updateInput(const float& dt);
	void updatePlayerInput(const float& dt);
	void updateTileMap(const float& dt);
	void update(const float& dt);
	void render(sf::RenderTarget* target = nullptr);
};

