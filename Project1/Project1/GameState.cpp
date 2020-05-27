#include "stdafx.h"
#include "GameState.h"


GameState::GameState(StateData* stateData) : State(stateData)
{
	initDeferredRender();
	initKeybinds();
	initTextures();
	initGUI();
	initTileMap();
	initPlayers();

	camera.setSize(1500 / 1.4, 900 / 1.4);
}


GameState::~GameState()
{
	std::cout << "- GameState Destroyed.\n";

	delete player;
	delete pausemenu;
	delete tileMap;
}

void GameState::loadNewMap(std::string filepath)
{
	//load in new maps
	tileMap->loadFromFile(filepath + ".tm");
	propMap->loadFromFile(filepath + ".pm", &fontMain);

	//find the spawn prop in the map
	for (auto& i : propMap->getMap())
	{
		if (i)
		{
			if (i->getID() == 4)
			{
				playerSpawn = i->getPosition();
			}
		}
	}

	player->setPosition(playerSpawn.x, playerSpawn.y);
}

void GameState::initKeybinds()
{

	std::ifstream fin("Properties/States/gamestate_keys.ini");

	if (fin.is_open())
	{
		std::string key = "";
		std::string keyVal = "";

		while (!fin.eof())
		{
			fin >> key >> keyVal;
			this->stateKeybinds[key] = this->keybinds->at(keyVal);
		}
	}

	fin.close();
}

void GameState::initTextures()
{
	textures["PLAYER"].loadFromFile("Assets/Textures/Entities/Player/player_sheet.png");
	textures["BUTTON"].loadFromFile("Assets/Textures/UI/mm_button.png");
}

void GameState::initGUI()
{
	pausemenu = new PauseMenu(*window, fontMain, textures);
	hud = new gui::HUD(&fontMain);
}

void GameState::initTileMap()
{
	tileMap = new TileMap(state_data->gridSize, 50, 50, 5, "Assets/Textures/Tiles/dungeonTileSheet.png");
	propMap = new PropMap(6000, 6000, "Assets/Textures/Tiles/dungeonPropSheet.png");

	tileMap->loadFromFile("Data/tilemaps/shop.tm");
	propMap->loadFromFile("Data/tilemaps/shop.pm", &fontMain);

	for (auto& i : propMap->getMap())
	{
		if (i)
		{
			if (i->getID() == 4)
			{
				playerSpawn = i->getPosition();
			}
		}
	}
}

void GameState::initDeferredRender()
{
	renderTexture.create(1500, 900);

	renderSprite.setTexture(renderTexture.getTexture());
	renderSprite.setTextureRect(sf::IntRect(0, 0, 1500, 900));
}

void GameState::initPlayers()
{
	player = new Player(playerSpawn.x, playerSpawn.y, textures["PLAYER"]);
	player->setScale(1.2, 1.2);
}

void GameState::updateView()
{
	// using std::floor on the position values to remove visual artifacts caused by moving the camera with float values.
	camera.setCenter(std::floor(player->getPosition().x + 63), std::floor(player->getPosition().y + 117));
}

void GameState::updateButtons(const sf::Vector2f& mousePosView)
{
	/*
		i should have made prop id's an enum, but i'm too lazy now

		prop ids:
			will render:
				- 0 = default
			wont render:
				- 1 = shopkeeper
				- 2 = load a random dungeon
				- 3 = load the shop
				- 4 = player spawn point
				- 5 = enemy spawn point
				- 6 = boss spawn point
	*/

	for (auto& i : propMap->getMap())
	{
		if (i)
		{
			if (i->isClickable())
			{
				i->update(mousePosView);

				//check if the button is pressed
				if ((i->getButton()->isPressed() && getKeyTime()))
				{
					// check if the player is within a certain distance from the button using pythagorean theorum
					if (sqrt(pow((i->getPosition().x - player->getPosition().x), 2) + pow((i->getPosition().y - player->getPosition().y), 2)) < 125)
					{
						// button id 6 loads dungeon 1
						if (i->getID() == 2)
						{
							loadNewMap("Data/tilemaps/dungeon1");
							player->stats.currentHealth = player->stats.maxHealth;
						}
						// button id 7 loads the shop
						else if (i->getID() == 3)
						{
							loadNewMap("Data/tilemaps/shop");
							player->stats.currentHealth = player->stats.maxHealth;
						}
					}
				}
			}
		}
	}
}

void GameState::updatePauseMenuButtons()
{
	if (pausemenu->getButtons()["EXIT"]->isPressed())
	{
		endState();
	}
}

void GameState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("CLOSE")) && getKeyTime())
	{
		if (!paused)
		{
			pauseState();
		}
		else
		{
			unpauseState();
		}
	}
}

void GameState::updatePlayerInput(const float& dt)
{
	if (!player->attacking)
	{
		if (sf::Keyboard::isKeyPressed(stateKeybinds.at("MOVE_LEFT"))) // left
		{
			player->move(-1.f, 0.f, dt);
		}
		if (sf::Keyboard::isKeyPressed(stateKeybinds.at("MOVE_RIGHT"))) // right
		{
			player->move(1.f, 0.f, dt);
		}
		if (sf::Keyboard::isKeyPressed(stateKeybinds.at("MOVE_UP"))) // forward
		{
			player->move(0.f, -1.f, dt);
		}
		if (sf::Keyboard::isKeyPressed(stateKeybinds.at("MOVE_DOWN"))) // back
		{
			player->move(0.f, 1.f, dt);
		}
	}

	if (player->getVelocity() != sf::Vector2f(0,0))
	{
		player->updateAngleDirection();
	}
}

void GameState::update(const float& dt)
{
	updateMousePositions(&camera);
	updateKeyTime(dt);
	updateInput(dt);

	if (!paused) //unpaused updates
	{
		updatePlayerInput(dt);
		updateButtons(mousePosView);
		player->update(dt);
		updateView();
		hud->update(player->getStats());
	}
	else //pause menu
	{
		pausemenu->update(mousePosWindow);
		updatePauseMenuButtons();
	}
}

void GameState::render(sf::RenderTarget* target)
{
	if (!target)
		target = window;

	renderTexture.clear();
	renderTexture.setView(camera);

	// three layers render below the player
	for (int i = 0; i < 3; i++)
	{
		tileMap->render(renderTexture, i);
	}

	propMap->render(renderTexture, false);
	player->render(renderTexture);

	// two layers render above the player
	for (int i = 3; i < 4; i++)
	{
		tileMap->render(renderTexture, i);
	}

	renderTexture.setView(renderTexture.getDefaultView());

	if (paused) // renders pause menu
	{
		pausemenu->render(renderTexture);
	}
	else
	{
		hud->render(renderTexture);
	}

	renderTexture.display();
	renderSprite.setTexture(renderTexture.getTexture());
	target->draw(renderSprite);
}
