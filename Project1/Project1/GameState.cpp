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

	//teleports the player to the new spawn location
	player->setPosition(playerSpawn.x, playerSpawn.y);
}

void GameState::initKeybinds()
{
	//establishes the state keybinds using a ini file
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
	//initializes the tilemap and propmap, and also sets the player spawn point for the new map.

	tileMap = new TileMap(state_data->gridSize, 50, 50, 5, "Assets/Textures/Tiles/dungeonTileSheet.png");
	propMap = new PropMap(6000, 6000, "Assets/Textures/Tiles/dungeonPropSheet.png");

	//loads the shop map by default
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
	//initializes the render texture.
	renderTexture.create(1500, 900);
	renderSprite.setTexture(renderTexture.getTexture());
	renderSprite.setTextureRect(sf::IntRect(0, 0, 1500, 900));
}

void GameState::initPlayers()
{
	player = new Player(playerSpawn.x, playerSpawn.y, textures["PLAYER"]);
	player->setScale(1.2, 1.2);
	
	//player facing forward at spawn
	player->move(0, -0.1, 1);
}

void GameState::updateView()
{
	// using std::floor on the position values to remove visual artifacts caused by moving the camera with float values.
	camera.setCenter((player->getPos().x + 63), (player->getPos().y + 117));
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
		if (i != nullptr)
		{
			if (i->isClickable())
			{
				//update buttons
				i->update(mousePosView);

				//check if the button is pressed
				if ((i->getButton()->isPressed() && getKeyTime()))
				{
					// check if the player is within a certain distance from the button using pythagorean theorum
					if (sqrt(pow((i->getPosition().x - player->getPos().x), 2) + pow((i->getPosition().y - player->getPos().y), 2)) < 125)
					{
						// button id 6 loads dungeon 1
						if (i->getID() == 2)
						{
							loadNewMap("Data/tilemaps/dungeon1");
							player->stats.currentHealth = player->stats.maxHealth;
							//after loading a new map the for loop has to break to stop it from trying to update a nullptr button.
							break;
						}
						// button id 7 loads the shop
						else if (i->getID() == 3)
						{
							loadNewMap("Data/tilemaps/shop");
							player->stats.currentHealth = player->stats.maxHealth;
							//after loading a new map the for loop has to break to stop it from trying to update a nullptr button.
							break;
						}
					}
				}
			}
		}
	}
}

void GameState::updatePauseMenuButtons()
{
	//updates exit button in pause menu
	if (pausemenu->getButtons()["EXIT"]->isPressed())
	{
		endState();
	}
}

void GameState::updateInput(const float& dt)
{
	//checks for a pause
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
	//player movement
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

	//updates the players face direction
	if (player->getVelocity() != sf::Vector2f(0,0))
	{
		player->updateAngleDirection();
	}
}

void GameState::updateTileMap(const float& dt)
{
	tileMap->update();
	tileMap->updateCollision(player, dt);
}

void GameState::update(const float& dt)
{
	//general update calls
	updateMousePositions(&camera);
	updateKeyTime(dt);
	updateInput(dt);

	if (!paused) //unpaused updates
	{
		updatePlayerInput(dt);
		updateButtons(mousePosView);
		updateView();
		hud->update(player->getStats());
		updateTileMap(dt);
		player->update(dt);
	}
	else //paused updates
	{
		pausemenu->update(mousePosWindow);
		updatePauseMenuButtons();
	}
}

void GameState::render(sf::RenderTarget* target)
{
	//general render calls
	if (!target)
		target = window;

	//clear the rendertexture
	renderTexture.clear();
	//set the view to the player camera.
	renderTexture.setView(camera);


	//renders first three layers of tilemap
	for (int i = 0; i < 3; i++)
	{
		tileMap->render(renderTexture, i, player);
	}

	//renders propmap and the player
	propMap->render(renderTexture, false);
	player->render(renderTexture);

	//renders top two layers of the tilemap
	for (int i = 3; i < 5; i++)
	{
		tileMap->render(renderTexture, i, player);
	}

	//set the view to the default view, this is useful for rendering GUI
	renderTexture.setView(renderTexture.getDefaultView());

	if (paused) // renders pause menu
	{
		pausemenu->render(renderTexture);
	}
	else
	{
		hud->render(renderTexture);
	}

	//displaying the render texture
	renderTexture.display();
	renderSprite.setTexture(renderTexture.getTexture());
	target->draw(renderSprite);
}
